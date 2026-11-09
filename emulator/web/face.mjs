const pageSize = 2048;
const rawPageSize = 2112;
const pagesPerBlock = 64;
const decoder = new TextDecoder();
const encoder = new TextEncoder();
const viewOf = bytes => new DataView(bytes.buffer, bytes.byteOffset, bytes.byteLength);
const align64 = size => Math.ceil(size / 64) * 64;

function sum32(bytes) {
  const view = viewOf(bytes);
  let sum = 0;
  for (let offset = 0; offset + 4 <= bytes.length; offset += 4) {
    sum = (sum + view.getUint32(offset, true)) >>> 0;
  }
  return sum;
}

function fontPages(nand) {
  if (nand.length !== rawPageSize * 65536) throw Error('Expected the prepared Watch Pro 2 NAND image.');
  const view = viewOf(nand);
  const blockSize = rawPageSize * pagesPerBlock;
  const blocks = new Map();
  for (let offset = 0; offset < nand.length; offset += blockSize) {
    const tag = offset + pageSize;
    const marker = view.getUint32(tag, true);
    const logical = view.getUint16(tag + 6, true);
    if ((marker !== 0xff00ffff && marker !== 0xff01ffff) || view.getUint16(tag + 4, true) ||
        logical < 0x34 || logical >= 0x34 + 96) continue;
    if (blocks.has(logical)) throw Error('Ambiguous font partition in virtual NAND.');
    blocks.set(logical, offset);
  }
  const pages = [];
  for (let logical = 0x34; blocks.has(logical); logical++) {
    for (let page = 0; page < pagesPerBlock; page++) {
      const offset = blocks.get(logical) + page * rawPageSize;
      const tag = offset + pageSize;
      if (page && (view.getUint32(tag, true) !== 0x5500ffff ||
          view.getUint16(tag + 4, true) !== page || view.getUint16(tag + 6, true) !== logical)) return pages;
      pages.push(offset);
    }
  }
  return pages;
}

function unpackSdfs(data) {
  if (data.length < pageSize || decoder.decode(data.subarray(0, 8)) !== 'sdfs.bin') {
    throw Error('Missing stock font and watch-face partition.');
  }
  const view = viewOf(data);
  const count = view.getUint32(12, true);
  const size = view.getUint32(16, true);
  const start = align64(32 + 32 * count);
  if (!count || count > 64 || size < start || size > data.length) throw Error('Invalid stock font partition.');
  const entries = new Map();
  for (let i = 0; i < count; i++) {
    const entry = 32 + i * 32;
    const name = decoder.decode(data.subarray(entry, entry + 12)).split('\0')[0];
    const offset = view.getUint32(entry + 12, true);
    const length = view.getUint32(entry + 16, true);
    const contents = data.subarray(offset, offset + length);
    if (offset < start || offset + length > size || entries.has(name) ||
        sum32(contents) !== view.getUint32(entry + 28, true)) {
      throw Error('Stock font partition failed its resource checks.');
    }
    entries.set(name, contents);
  }
  return entries;
}

function packSdfs(entries, capacity) {
  const start = align64(32 + entries.size * 32);
  const size = start + Array.from(entries.values()).reduce((total, data) => total + align64(data.length), 0);
  if (size > capacity) return null;
  const packed = new Uint8Array(size);
  const view = viewOf(packed);
  packed.set(encoder.encode('sdfs.bin'));
  view.setUint32(12, entries.size, true);
  view.setUint32(16, size, true);
  let entry = 32;
  let offset = start;
  for (const [name, contents] of entries) {
    packed.set(encoder.encode(name), entry);
    packed.set(contents, offset);
    view.setUint32(entry + 12, offset, true);
    view.setUint32(entry + 16, contents.length, true);
    view.setUint32(entry + 28, sum32(contents), true);
    entry += 32;
    offset += align64(contents.length);
  }
  view.setUint32(24, sum32(packed.subarray(32, entry)), true);
  view.setUint32(28, sum32(packed.subarray(start)), true);
  return packed;
}

function unpackLz4(source, length) {
  const output = new Uint8Array(length);
  let cursor = 0;
  let position = 0;
  const read = () => {
    if (cursor >= source.length) throw Error('Truncated watch-face texture.');
    return source[cursor++];
  };
  const runLength = initial => {
    let size = initial;
    if (initial === 15) {
      let next;
      do {
        next = read();
        size += next;
      } while (next === 255);
    }
    return size;
  };
  while (cursor < source.length) {
    const token = read();
    const literals = runLength(token >>> 4);
    if (cursor + literals > source.length || position + literals > length) throw Error('Invalid watch-face texture.');
    output.set(source.subarray(cursor, cursor + literals), position);
    position += literals;
    cursor += literals;
    if (cursor === source.length) break;
    const distance = read() | (read() << 8);
    const match = runLength(token & 15) + 4;
    if (!distance || distance > position || position + match > length) throw Error('Invalid watch-face texture.');
    for (let i = 0; i < match; i++, position++) output[position] = output[position - distance];
  }
  if (position !== length) throw Error('Incomplete watch-face texture.');
  return output;
}

function rawCrc32(bytes) {
  let crc = 0;
  for (const byte of bytes) {
    crc ^= byte;
    for (let bit = 0; bit < 8; bit++) crc = (crc >>> 1) ^ ((crc & 1) ? 0xedb88320 : 0);
  }
  return crc >>> 0;
}

async function faceTextures(face) {
  const view = viewOf(face);
  const end = face.length - 36;
  if (face.length < 75 || face.length > 3021359 || view.getUint32(4, true) !== (1 + (face[7] === 2 ? 0x02000000 : 0)) ||
      face[36] !== 0x20 || !face.subarray(0, 36).every((byte, index) => byte === face[end + index])) {
    throw Error('Expected a Watch Pro 2 watch-face file.');
  }
  let offset = 39 + view.getUint16(37, true);
  if (offset > end || view.getUint32(24, true) !== end ||
      view.getUint32(28, true) !== end - offset ||
      rawCrc32(face.subarray(4, offset)) !== view.getUint32(0, true) ||
      rawCrc32(face.subarray(offset, end)) !== view.getUint32(32, true)) {
    throw Error('Watch-face checksums or section lengths are invalid.');
  }
  const textures = new Set();
  while (offset < end) {
    if (end - offset < 8) throw Error('Truncated watch-face resource.');
    const header = view.getUint32(offset, true);
    const size = view.getUint32(offset + 4, true);
    const width = (header >>> 10) & 2047;
    const height = header >>> 21;
    offset += 8;
    if (!width || !height) throw Error('A watch-face texture has zero width or height.');
    if (offset + size > end) throw Error('A watch-face texture extends past the end of the file.');
    if ((header & 31) === 5) {
      const pixels = unpackLz4(face.subarray(offset, offset + size), width * height * 3);
      const digest = new Uint8Array(await crypto.subtle.digest('SHA-256', pixels));
      const hash = Array.from(digest, byte => byte.toString(16).padStart(2, '0')).join('');
      textures.add(`${width}:${height}:${hash}`);
    }
    offset += size;
  }
  return [...textures].sort().map(texture => texture + '\n').join('');
}

export async function installWatchFace(nand, face) {
  const index = await faceTextures(face);
  const pages = fontPages(nand);
  const data = new Uint8Array(pages.length * pageSize);
  pages.forEach((offset, page) => data.set(nand.subarray(offset, offset + pageSize), page * pageSize));
  const entries = unpackSdfs(data);
  const defaultFace = entries.get('local273.wfc');
  if (!defaultFace) throw Error('The default Activity Mood slot is missing.');
  entries.set('local273.wfc', face);
  let packed = packSdfs(entries, data.length);
  if (!packed && entries.has('local277.wfc')) {
    entries.set('local277.wfc', defaultFace);
    packed = packSdfs(entries, data.length);
  }
  if (!packed) throw Error('This face exceeds the space in the prepared NAND fixture.');
  for (let start = 0; start < packed.length; start += pageSize) {
    nand.set(packed.subarray(start, start + pageSize), pages[start / pageSize]);
  }
  return index;
}
