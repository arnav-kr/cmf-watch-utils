const $ = (id) => document.getElementById(id);
const panel = $('panel');
const context = panel.getContext('2d', {alpha:false});
const pixels = context.createImageData(466,466);
const hosted = document.body.hasAttribute('data-hosted');
const fixtureFolder = $('fixture-folder');
const requiredFixtures = ['nor.bin', 'nand-defaults.bin', 'nand-id.bin', 'compat.bin', 'app.bin'];
let runtime, timer, pointer=null, crownDown=false, frames=0, lastFrame=null;
let customApp = false;
let runtimeLog='', halted=false, nextLog=0;
const debug=new URLSearchParams(location.search).has('debug');
const encoder = new TextEncoder();
const fatalGuestOutput = /ZEPHYR FATAL ERROR|\bHARD FAULT\b|Kernel panic|Lockup: can't escalate/i;
function status(text,error=false,state) {
  const message = $('status');
  message.textContent = text;
  message.toggleAttribute('data-error', error);
  $('notification').hidden = !error && state !== 'loading' && state !== 'timeout';
  $('recover').hidden = !error && state !== 'timeout';
}
function guestStopped(message) {
  if (halted) return;
  halted = true;
  status(message, true);
  for (const button of document.querySelectorAll('#crown, #up, #down')) {
    button.disabled = true;
  }
}

function bootStock() {
  const url = new URL(location.href);
  url.searchParams.set('recover', 'stock');
  location.replace(url.href);
}

$('recover').onclick = bootStock;
$('hard-reset').onclick = bootStock;

function log(text) {
  runtimeLog=(runtimeLog+'\n'+text).slice(-8000);
  $('log').textContent=runtimeLog;
  if (!halted && fatalGuestOutput.test(text)) {
    guestStopped('Guest firmware crashed. Save the watch log or boot stock.');
  } else if (!halted && /stopped on unimplemented|Unsupported|Aborted|Invalid WFC|Cannot read WFC/.test(text)) {
    guestStopped('Emulator stopped. See the watch log.');
  }
}
function input(line) {
  if (!runtime||halted) return;
  const bytes=encoder.encode(line+'\n');
  const stream=runtime.FS.open('/watch/input.txt','a');
  try {runtime.FS.write(stream,bytes,0,bytes.length);} finally {runtime.FS.close(stream);}
}
function crown(down) {
  if(crownDown===down)return;
  crownDown=down; input(`c ${down?1:0}`);
}
function point(event) {
  const rect=panel.getBoundingClientRect();
  let x=(event.clientX-rect.left)*466/rect.width,y=(event.clientY-rect.top)*466/rect.height;
  const dx=x-233,dy=y-233,r=Math.hypot(dx,dy);
  if(r>232){x=233+dx*232/r;y=233+dy*232/r;}
  return [Math.round(x),Math.round(y)];
}
function release() {
  if(pointer){input(`t 0 ${pointer.x} ${pointer.y}`);pointer=null;}
  crown(false);
}
panel.addEventListener('pointerdown',event=>{
  if(!runtime||pointer)return;
  const [x,y]=point(event);pointer={id:event.pointerId,x,y};
  panel.setPointerCapture(event.pointerId);panel.focus();input(`t 1 ${x} ${y}`);
});
panel.addEventListener('pointermove',event=>{
  if(!pointer||pointer.id!==event.pointerId)return;
  const [x,y]=point(event);
  if(x!==pointer.x||y!==pointer.y){Object.assign(pointer,{x,y});input(`t 1 ${x} ${y}`);}
});
for(const name of ['pointerup','pointercancel','lostpointercapture']) panel.addEventListener(name,release);
$('crown').addEventListener('pointerdown',event=>{event.preventDefault();$('crown').setPointerCapture(event.pointerId);crown(true);});
for(const name of ['pointerup','pointercancel','lostpointercapture']) $('crown').addEventListener(name,()=>crown(false));
$('up').onclick=()=>input('r 80');$('down').onclick=()=>input('r -80');
panel.addEventListener('wheel',event=>{if(runtime){event.preventDefault();input(`r ${event.deltaY>0?-80:80}`);}},{passive:false});
window.addEventListener('keydown',event=>{
  if(!runtime||halted||event.ctrlKey||event.altKey||event.metaKey||!['Space','ArrowUp','ArrowDown'].includes(event.code))return;
  if(event.target.closest('button,input,select,summary,a'))return;
  event.preventDefault();
  if(event.code==='Space')crown(true);
  else input(`r ${event.code==='ArrowUp'?80:-80}`);
});
window.addEventListener('keyup',event=>{if(event.code==='Space' && crownDown){event.preventDefault();crown(false);}});
window.addEventListener('blur',release);
document.addEventListener('visibilitychange',()=>{if(document.hidden)release();});
function renderFrame() {
  try {
    const data=runtime.FS.readFile('/watch/frame.ppm');
    const header='P6\n466 466\n255\n';
    if(data.length!==header.length+466*466*3)return;
    for(let i=0;i<header.length;i++)if(data[i]!==header.charCodeAt(i))throw Error('Unexpected panel format');
    if(lastFrame&&data.every((v,i)=>v===lastFrame[i]))return;
    lastFrame=data;
    for(let i=0,j=header.length;i<pixels.data.length;i+=4,j+=3){
      pixels.data[i]=data[j];pixels.data[i+1]=data[j+1];pixels.data[i+2]=data[j+2];pixels.data[i+3]=255;
    }
    context.putImageData(pixels,0,0);frames++;
    $('offline').hidden=true;
    $('capture').disabled = false;
    if(!halted)status('Firmware is rendering locally.',false,'running');
  } catch(error) {if(error.name!=='ErrnoError'){log(error.message);clearInterval(timer);}}
}
function readLog(path,limit) {
  const length=runtime.FS.stat(path).size;
  const stream=runtime.FS.open(path,'r');
  try {
    const bytes=new Uint8Array(Math.min(length,limit));
    runtime.FS.read(stream,bytes,0,bytes.length,Math.max(0,length-limit));
    return new TextDecoder().decode(bytes);
  } finally {runtime.FS.close(stream);}
}
function render() {
  renderFrame();
  if(performance.now()<nextLog)return;
  nextLog=performance.now()+500;
  try {
    const uart=readLog('/watch/uart.log',32000).split('\n').filter(line=>!line.includes('#CD:')).join('\n');
    let diagnostic='';
    if(debug)try{diagnostic=readLog('/watch/qemu.log',12000);}catch{}
    const logPanel = $('log');
    const output = runtimeLog + '\n' + diagnostic + '\n' + uart;
    if (logPanel.textContent !== output) {
      const follow = logPanel.scrollHeight - logPanel.scrollTop - logPanel.clientHeight <= 4;
      logPanel.textContent = output;
      if (follow) logPanel.scrollTop = logPanel.scrollHeight;
    }
    if (!halted && fatalGuestOutput.test(uart)) {
      guestStopped('Guest firmware crashed. Save the watch log or boot stock.');
    }
  } catch {}
}
const frameColor = $('frame-color');
const frameCase = $('case');
function loadFrame() {
  const color = frameColor.value;
  const image = new Image();
  image.onload = () => {
    if (frameColor.value !== color) return;
    frameCase.dataset.frame = color;
    frameCase.style.setProperty('--frame-image', `url("${image.src}")`);
  };
  image.onerror = () => {
    if (frameColor.value === color) delete frameCase.dataset.frame;
  };
  image.src = `./frames/${color}.png`;
}
frameColor.addEventListener('change', loadFrame);
if (hosted) {
  $('hosted-fixtures').hidden = false;
  $('firmware-package').hidden = true;
  $('watch-face-upload').hidden = true;
  frameColor.closest('label').hidden = true;
  document.querySelector('.wordmark').href = '../';
} else {
  loadFrame();
}
$('show-frame').onchange=event=>$('case').classList.toggle('frameless',!event.target.checked);
const device = document.querySelector('.device-card');
function updateFullscreen() {
  const expanded = document.fullscreenElement === device || device.classList.contains('expanded');
  const label = expanded ? 'Exit fullscreen' : 'Expand watch view';
  $('fullscreen').setAttribute('aria-label', label);
  $('fullscreen').title = label;
}

$('fullscreen').onclick = async () => {
  if (device.classList.contains('expanded')) {
    device.classList.remove('expanded');
  } else if (document.fullscreenElement === device) {
    await document.exitFullscreen();
  } else {
    try {
      await device.requestFullscreen();
    } catch {
      device.classList.add('expanded');
    }
  }
  updateFullscreen();
};
document.addEventListener('fullscreenchange', updateFullscreen);
document.addEventListener('keydown', event => {
  if (event.key === 'Escape' && device.classList.contains('expanded')) {
    device.classList.remove('expanded');
    updateFullscreen();
  }
});
$('save-log').onclick=()=>{
  const parts = [runtimeLog, '\n'];
  if (runtime) {
    for (const path of debug ? ['/watch/qemu.log', '/watch/uart.log'] : ['/watch/uart.log']) {
      try { parts.push(runtime.FS.readFile(path), '\n'); } catch {}
    }
  }
  const url = URL.createObjectURL(new Blob(parts, {type:'text/plain'}));
  const link=document.createElement('a');link.href=url;link.download='wp2-browser.log';link.click();
  setTimeout(()=>URL.revokeObjectURL(url),1000);
};
const roller = document.querySelector('.knurls');
let rollY;
roller.addEventListener('pointerdown', event => {
  if (!runtime || halted) return;
  rollY = event.clientY;
  roller.setPointerCapture(event.pointerId);
});
roller.addEventListener('pointermove', event => {
  if (rollY === undefined || !roller.hasPointerCapture(event.pointerId)) return;
  const steps = Math.trunc((event.clientY - rollY) / 12);
  if (!steps) return;
  input(`r ${Math.max(-320, Math.min(320, -steps * 40))}`);
  rollY += steps * 12;
});
roller.addEventListener('lostpointercapture', () => { rollY = undefined; });
roller.addEventListener('wheel', event => {
  if (!runtime || !event.deltaY) return;
  event.preventDefault();
  input(event.deltaY > 0 ? 'r -80' : 'r 80');
}, { passive: false });
for (const picker of document.querySelectorAll('.file-control input')) {
  const label = picker.parentElement.querySelector('.file-name');
  const clear = picker.closest('.file-row').querySelector('.clear-file');
  picker.addEventListener('change', () => {
    const file = picker.files[0];
    const name = picker === fixtureFolder ? file?.webkitRelativePath.split('/')[0] : file?.name;
    label.textContent = name || (picker === fixtureFolder ? 'Select folder' : 'Select file');
    label.title = name || '';
    clear.hidden = !file;
    $('image-error').textContent = '';
  });
  clear.onclick = () => {
    if (picker.disabled) return;
    picker.value = '';
    picker.dispatchEvent(new Event('change'));
    picker.focus();
  };
}

let powerChanged = false;
let powerTimer;
function sendPower() {
  const percent = Number($('battery').value);
  const voltage = percent <= 69
    ? 3300 + 700 * percent / 69
    : 4000 + 350 * (percent - 69) / 31;
  input(`p ${Math.round(voltage)} ${Number($('charger').checked)}`);
}
function updateBattery() {
  const percent = $('battery').value + '%';
  $('battery-level').value = percent;
  $('battery').style.setProperty('--level', percent);
}
$('battery').oninput = () => {
  powerChanged = true;
  updateBattery();
  if (!powerTimer) {
    powerTimer = setTimeout(() => {
      powerTimer = undefined;
      sendPower();
    }, 50);
  }
};
$('charger').onchange = () => {
  powerChanged = true;
  sendPower();
};
updateBattery();

function updateBrightness() {
  const brightness = $('brightness');
  const percent = brightness.value + '%';
  $('brightness-level').value = percent;
  brightness.style.setProperty('--level', percent);
  input(`b ${brightness.value}`);
}
$('brightness').oninput = updateBrightness;
updateBrightness();

function sendHealth() {
  if (customApp) return;
  if (!$('health-enabled').checked) {
    input('h 0 72 98');
    return;
  }
  const rate = $('heart-rate');
  const oxygen = $('blood-oxygen');
  if (rate.validity.valid && oxygen.validity.valid) {
    input(`h 1 ${rate.value} ${oxygen.value}`);
  }
}

function sendGps() {
  if (!$('gps-enabled').checked) {
    input('g 0 0 0');
    return;
  }
  const latitude = $('gps-lat');
  const longitude = $('gps-lon');
  if (latitude.validity.valid && longitude.validity.valid) {
    input(`g 1 ${Math.round(latitude.value * 1e6)} ${Math.round(longitude.value * 1e6)}`);
  }
}

for (const id of ['health-enabled', 'heart-rate', 'blood-oxygen']) $(id).onchange = sendHealth;
for (const id of ['gps-enabled', 'gps-lat', 'gps-lon']) $(id).onchange = sendGps;

function buildCylinder(element, radius, depth, facets, ridged) {
  const width = 2 * radius * Math.tan(Math.PI / facets) + 0.35;
  for (let index = 0; index < facets; index++) {
    const side = document.createElement('div');
    const angle = index * 360 / facets;
    const light = 17 + 21 * Math.max(0, Math.cos((angle - 325) * Math.PI / 180))
      + (ridged && index % 2 === 0 ? 11 : 0);
    side.className = 'cylinder-side';
    side.style.width = `${width}px`;
    side.style.height = `${depth}px`;
    side.style.background = `linear-gradient(hsl(0 0% ${light + 9}%), hsl(0 0% ${light}%) 25% 75%, hsl(0 0% ${light - 5}%))`;
    side.style.transform = `translate(-50%, -50%) rotateZ(${angle}deg) translateY(-${radius}px) rotateX(90deg)`;
    element.append(side);
  }
}

buildCylinder($('orientation-model'), 27, 9, 36, false);
buildCylinder(document.querySelector('.crown-cylinder'), 4, 6, 20, true);

const orientationPad = $('orientation-pad');
let pitch = 0;
let roll = 0;
let gravity = [0, 0, 1000];
let drag;
let motionTimer;

const axes = [...$('orientation-axes').querySelectorAll('input')];

function showOrientation() {
  $('orientation-model').style.transform = `rotateY(${roll}deg) rotateX(${pitch}deg)`;
  $('orientation-reading').textContent = `X ${gravity[0]} · Y ${gravity[1]} · Z ${gravity[2]} mg`;
  if (!motionTimer) {
    motionTimer = setTimeout(() => {
      motionTimer = undefined;
      input(`a ${gravity.join(' ')}`);
    }, 50);
  }
}

function setOrientation(nextPitch, nextRoll) {
  pitch = nextPitch % 360;
  roll = nextRoll % 360;
  const x = pitch * Math.PI / 180;
  const y = roll * Math.PI / 180;
  gravity = [-Math.sin(y), Math.cos(y) * Math.sin(x), Math.cos(y) * Math.cos(x)]
    .map(value => Math.round(value * 1000) || 0);
  axes.forEach((axis, index) => { axis.value = gravity[index]; });
  showOrientation();
}

$('orientation-reading').onclick = () => {
  const editor = $('orientation-axes');
  editor.hidden = !editor.hidden;
  $('orientation-reading').setAttribute('aria-expanded', String(!editor.hidden));
  if (!editor.hidden) axes[0].focus();
};

for (const axis of axes) {
  axis.addEventListener('input', () => {
    if (axes.some(field => !field.value || !field.validity.valid)) return;
    gravity = axes.map(field => Number(field.value));
    const [x, y, z] = gravity;
    const magnitude = Math.hypot(x, y, z);
    if (magnitude) {
      roll = Math.asin(-x / magnitude) * 180 / Math.PI;
      pitch = Math.atan2(y, z) * 180 / Math.PI;
    }
    showOrientation();
  });
  axis.addEventListener('blur', () => {
    if (!axis.value || !axis.validity.valid) axis.value = gravity[axes.indexOf(axis)];
  });
}

orientationPad.addEventListener('pointerdown', event => {
  if (event.button !== 0 || drag) return;
  orientationPad.focus();
  orientationPad.setPointerCapture(event.pointerId);
  drag = {id: event.pointerId, x: event.clientX, y: event.clientY, pitch, roll};
});
orientationPad.addEventListener('pointermove', event => {
  if (drag?.id !== event.pointerId) return;
  setOrientation(drag.pitch - (event.clientY - drag.y) * 2,
    drag.roll + (event.clientX - drag.x) * 2);
});
orientationPad.addEventListener('lostpointercapture', () => { drag = undefined; });
orientationPad.addEventListener('keydown', event => {
  switch (event.key) {
    case 'ArrowUp': setOrientation(pitch + 5, roll); break;
    case 'ArrowDown': setOrientation(pitch - 5, roll); break;
    case 'ArrowLeft': setOrientation(pitch, roll - 5); break;
    case 'ArrowRight': setOrientation(pitch, roll + 5); break;
    case 'Home': setOrientation(0, 0); break;
    default: return;
  }
  event.preventDefault();
  event.stopPropagation();
});
$('reset-orientation').onclick = () => setOrientation(0, 0);
$('reset-simulation').onclick = () => {
  $('brightness').value = 100;
  updateBrightness();
  $('battery').value = 69;
  $('charger').checked = false;
  $('health-enabled').checked = false;
  $('heart-rate').value = 72;
  $('blood-oxygen').value = 98;
  $('gps-enabled').checked = false;
  $('gps-lat').value = $('gps-lon').value = 0;
  sendHealth();
  sendGps();
  powerChanged = true;
  updateBattery();
  sendPower();
  setOrientation(0, 0);
};

$('capture').onclick = () => {
  if (!frames) return;
  panel.toBlob(blob => {
    if (!blob) return;
    const url = URL.createObjectURL(blob);
    const link = document.createElement('a');
    link.href = url;
    link.download = 'watch-pro-2.png';
    link.click();
    setTimeout(() => URL.revokeObjectURL(url), 1000);
  });
};

async function readBootOverrides() {
  const firmware = $('firmware-file').files[0];
  const app = $('app-file').files[0];
  const nand = $('nand-file').files[0];
  const face = $('face-file').files[0];
  const sdfs = $('sdfs-file').files[0];
  if (firmware && (app || sdfs || face)) {
    throw Error('Clear the raw application and watch-face inputs before using a firmware package.');
  }
  if (face && (app || nand)) {
    throw Error('Use the watch-face picker with the stock application and default NAND.');
  }
  if (sdfs && (!app || sdfs.size < 32 || sdfs.size > 0x10000 ||
      await sdfs.slice(0, 8).text() !== 'sdfs.bin')) {
    throw Error('Choose a companion SDFS image up to 64 KiB with a custom application.');
  }
  if (nand && nand.size !== 2112 * 65536) {
    throw Error('Virtual NAND must be a 138,412,032-byte prepared raw image.');
  }
  if (firmware) {
    if (firmware.size > 64 * 1024 ** 2) throw Error('Firmware packages must be 64 MiB or smaller.');
    let packageBytes;
    try {
      packageBytes = await firmware.arrayBuffer();
    } catch {
      throw Error('The selected firmware file could not be read. Select a local copy and retry.');
    }
    if (packageBytes.byteLength === 56903752) {
      const hash = await crypto.subtle.digest('SHA-256', packageBytes);
      const digest = Array.from(new Uint8Array(hash), byte => byte.toString(16).padStart(2, '0')).join('');
      if (digest === '30903fc8ce6db4f8860bb554366635db3d2bcbb2d43827b3b50fbbac22b59353') return { nand };
    }
    let response;
    try {
      response = await fetch('./firmware', {
        method: 'POST',
        headers: {'Content-Type': 'application/octet-stream'},
        body: packageBytes,
      });
    } catch {
      throw Error('The local firmware importer did not respond. Check that the emulator server is running and retry.');
    }
    if ([404, 405, 501].includes(response.status)) {
      throw Error('This server has no firmware importer. Open the emulator served by the current serve.py.');
    }
    if (response.status === 204) return { nand };
    if (!response.ok) throw Error(`Firmware package rejected: ${response.statusText}`);
    const payload = await response.arrayBuffer();
    const length = new DataView(payload).getUint32(0, true);
    if (length < 0x208 || length > 0x34b000 || payload.byteLength - 4 - length <= 0 ||
        payload.byteLength - 4 - length > 0x10000) {
      throw Error('Invalid application or resources returned by the firmware importer.');
    }
    return {
      nand,
      app: new Uint8Array(payload, 4, length),
      sdfs: new Blob([new Uint8Array(payload, 4 + length)]),
    };
  }
  if (!app) return { nand, face };
  if (app.size < 0x208 || app.size > 0x34b000) {
    throw Error('Application does not fit the Watch Pro 2 partition.');
  }
  const bytes = new Uint8Array(await app.arrayBuffer());
  const vectors = new DataView(bytes.buffer);
  const sp = vectors.getUint32(0x200, true);
  const reset = vectors.getUint32(0x204, true);
  if (new TextDecoder().decode(bytes.subarray(0, 8)) !== 'ACTHHTCA' ||
      sp < 0x2ff00000 || sp >= 0x30000000 || !(reset & 1) ||
      reset < 0x10000200 || reset >= 0x10000000 + bytes.length) {
    throw Error('Expected a Watch Pro 2 ACTH application with valid boot vectors.');
  }
  return { app: bytes, nand, sdfs };
}

async function installFace(face, files) {
  const response = await fetch('./watch-face', {
    method: 'POST',
    headers: { 'Content-Type': 'application/octet-stream' },
    body: face,
  });
  if (!response.ok) throw Error(`Watch-face import failed: ${response.statusText}`);
  const patch = new Uint8Array(await response.arrayBuffer());
  const view = new DataView(patch.buffer);
  const indexSize = view.getUint32(0, true);
  const nand = files.find(([name]) => name === 'nand-defaults.bin')[1];
  let cursor = 4 + indexSize;
  if (cursor > patch.length) throw Error('Invalid watch-face texture index.');
  while (cursor < patch.length) {
    const offset = view.getUint32(cursor, true);
    const size = view.getUint32(cursor + 4, true);
    cursor += 8;
    if (size > 2048 || offset + size > nand.length || cursor + size > patch.length) {
      throw Error('Invalid watch-face NAND patch.');
    }
    nand.set(patch.subarray(cursor, cursor + size), offset);
    cursor += size;
  }
  const existing = files.find(([name]) => name === 'wfc-index.txt');
  if (existing) {
    const merged = new Uint8Array(existing[1].length + 1 + indexSize);
    merged.set(existing[1]);
    merged[existing[1].length] = 10;
    merged.set(patch.subarray(4, 4 + indexSize), existing[1].length + 1);
    existing[1] = merged;
  } else if (indexSize) {
    files.push(['wfc-index.txt', patch.subarray(4, 4 + indexSize)]);
  }
}

async function loadFixtures(nandOverride) {
  if (hosted) {
    const selected = new Map([...fixtureFolder.files]
      .filter(file => file.webkitRelativePath.split('/').length === 2)
      .map(file => [file.name, file]));
    const missing = requiredFixtures.filter(name => !selected.has(name));
    if (missing.length) throw Error(`The selected folder is missing ${missing.join(', ')}.`);
    const names = [...requiredFixtures];
    if (selected.has('wfc-index.txt')) names.push('wfc-index.txt');
    return Promise.all(names.map(async name => {
      const file = name === 'nand-defaults.bin' && nandOverride ? nandOverride : selected.get(name);
      return [name, new Uint8Array(await file.arrayBuffer())];
    }));
  }
  const response = await fetch('./fixtures/manifest.json');
  if (!response.ok) throw Error('Prepare the local fixture manifest first.');
  const manifest = await response.json();
  return Promise.all(manifest.files.map(async name => {
    if (name === 'nand-defaults.bin' && nandOverride) {
      return [name, new Uint8Array(await nandOverride.arrayBuffer())];
    }
    const file = await fetch(`./fixtures/${name}`);
    if (!file.ok) throw Error(`Missing ${name}`);
    return [name, new Uint8Array(await file.arrayBuffer())];
  }));
}

$('stop').onclick=()=>location.reload();
$('start').onclick=async()=>{
  if (hosted && !fixtureFolder.files.length) {
    $('image-error').textContent = 'Select your stock fixtures folder before pressing Play.';
    fixtureFolder.focus();
    return;
  }
  $('start').disabled=true;
  try {
    if(!crossOriginIsolated)throw Error('Cross-origin isolation is required. Start the supplied local server.');
    $('image-error').textContent = '';
    const pickers = ['fixture-folder', 'firmware-file', 'app-file', 'sdfs-file', 'face-file', 'nand-file'];
    for (const id of pickers) $(id).disabled = true;
    if ($('firmware-file').files.length) status('Checking firmware package...', false, 'loading');
    let overrides, files;
    try {
      overrides = await readBootOverrides();
      status('Loading the runtime and virtual storage...', false, 'loading');
      files = await loadFixtures(overrides.nand);
    } catch (error) {
      $('image-error').textContent = error.message;
      for (const id of pickers) $(id).disabled = false;
      status('Ready');
      $('start').disabled = false;
      return;
    }
    customApp = Boolean(overrides.app);
    for (const field of $('health-controls').querySelectorAll('input')) field.disabled = customApp;
    $('firmware-label').textContent = customApp ? 'CUSTOM APPLICATION' : 'FIRMWARE 1.0.0.73';
    $('stop').disabled=false;
    if (overrides.face) {
      try {
        await installFace(overrides.face, files);
      } catch (error) {
        $('image-error').textContent = error.message;
        for (const id of pickers) $(id).disabled = false;
        $('start').disabled = false;
        $('stop').disabled = true;
        status('Choose a watch face and try again.', false, 'ready');
        return;
      }
    }
    if (customApp) {
      const nor = files.find(([name]) => name === 'nor.bin')[1];
      nor.fill(0xff, 0x24000, 0x36f000);
      nor.set(overrides.app, 0x24000);
      if (overrides.sdfs) {
        nor.fill(0xff, 0x3d3000, 0x3e3000);
        nor.set(new Uint8Array(await overrides.sdfs.arrayBuffer()), 0x3d3000);
      }
    }
    const {default:createQemu}=await import('./runtime/qemu-system-arm.js');
    runtime=await createQemu({
      mainScriptUrlOrBlob:new URL('./runtime/qemu-system-arm.js',location.href).href,
      locateFile:name=>new URL(`./runtime/${name}`,location.href).href,
      print:log,printErr:log,onAbort:reason=>guestStopped(reason ? `Runtime aborted: ${reason}` : 'Runtime aborted. See the watch log.'),
      onExit:code=>guestStopped(`Emulator exited (${code}). See the watch log.`),
      arguments:['-M','ats3089c-wp2','-bios','/watch/compat.bin',
        ...(!customApp ? ['-kernel', '/watch/app.bin'] : []),
        ...(debug?['-d','unimp','-D','/watch/qemu.log']:[]),
        '-display','none','-monitor','none','-serial','file:/watch/uart.log','-serial','null','-serial','null',
        '-icount','shift=3,align=off','-no-reboot'],
      preRun:[module=>{
        module.FS.mkdir('/watch');
        for(const [name,data]of files)module.FS.writeFile(`/watch/${name}`,data);
        module.FS.writeFile('/watch/input.txt','');
        Object.assign(module.ENV,{QEMU_WP2_NOR:'/watch/nor.bin',QEMU_WP2_NAND:'/watch/nand-defaults.bin',
          QEMU_WP2_FRAME:'/watch/frame.ppm',QEMU_WP2_INPUT:'/watch/input.txt',QEMU_WP2_LIVE_FRAME:'1',
          QEMU_WP2_LIVE_SENSORS:'1',QEMU_WP2_HOST_RTC:'1',QEMU_WP2_UNPLUGGED:'1'});
        if (!customApp) {
          module.ENV.QEMU_WP2_LIVE_HEALTH = '1';
          module.ENV.QEMU_WP2_STOCK_AWAKE = '1';
        }
        if(files.some(([name]) => name === 'wfc-index.txt')&&!new URLSearchParams(location.search).has('no-textures'))module.ENV.QEMU_WP2_WFC_INDEX='/watch/wfc-index.txt';
      }]
    });
    if (!halted) {
      for (const id of ['crown','up','down']) $(id).disabled = false;
      if (powerChanged) sendPower();
      updateBrightness();
      if ($('health-enabled').checked) sendHealth();
      if ($('gps-enabled').checked) sendGps();
      input(`a ${gravity.join(' ')}`);
      status('Waiting for the guest display...', false, 'loading');
      setTimeout(() => {
        if (!halted && !frames) status('No display frame after 90 seconds. Check the watch log.', false, 'timeout');
      }, 90000);
    }
    timer = setInterval(render, 50);
  }catch(error){log(error.stack);guestStopped(error.message);}
};


const bootUrl = new URL(location.href);
if (hosted && !crossOriginIsolated) {
  $('start').disabled = true;
  if (!navigator.serviceWorker || bootUrl.searchParams.has('isolation-reload')) {
    status('This browser could not start the emulator workers. Try a recent desktop version of Chrome or Edge.', true);
  } else {
    status('Preparing browser emulation...', false, 'loading');
    const reloadIsolated = () => {
      bootUrl.searchParams.set('isolation-reload', '1');
      location.replace(bootUrl);
    };
    navigator.serviceWorker.addEventListener('controllerchange', reloadIsolated, {once: true});
    navigator.serviceWorker.register('./isolation-worker.js').then(() => {
      if (navigator.serviceWorker.controller) reloadIsolated();
    }).catch(() => {
      status('The browser blocked the emulator worker. Check that service workers are allowed for this site.', true);
    });
  }
} else if (bootUrl.searchParams.has('isolation-reload')) {
  bootUrl.searchParams.delete('isolation-reload');
  history.replaceState(null, '', bootUrl);
}

if (bootUrl.searchParams.get('recover') === 'stock') {
  bootUrl.searchParams.delete('recover');
  history.replaceState(null, '', bootUrl);
  for (const picker of document.querySelectorAll('input[type=file]')) picker.value = '';
  if (hosted) {
    $('image-error').textContent = 'Session reset. Select the stock fixtures folder again, then press Play.';
  } else {
    $('start').click();
  }
}
