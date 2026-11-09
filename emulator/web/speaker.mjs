export const speakerHop = rate => Math.round(rate * .012);

export function speakerSegment(read, first, end, cursor, overlap, rate, flush) {
  const hop = speakerHop(rate);
  const radius = Math.round(rate * .005);
  const wanted = Math.round(cursor);
  if (wanted >= end || (!flush && wanted + 2 * hop > end)) return;
  const lower = Math.max(first, wanted - radius);
  const upper = Math.min(wanted + radius, flush ? end - 1 : end - 2 * hop);
  if (lower > upper) return;
  const sample = index => index < end ? read(index) : 0;
  let start = Math.max(lower, Math.min(wanted, upper));
  if (overlap) {
    const stride = Math.max(1, Math.round(rate / 8000));
    let best = -Infinity;
    const compare = candidate => {
      let sum = 0, reference = 0, power = 0;
      for (let i = 0; i < hop; i += stride) {
        const a = overlap[i], b = sample(candidate + i);
        sum += a * b;
        reference += a * a;
        power += b * b;
      }
      const similarity = sum / Math.sqrt(reference * power + 1e-12);
      const score = similarity - .08 * Math.abs(candidate - wanted) / radius;
      if (score > best) {
        start = candidate;
        best = score;
      }
    };
    compare(start);
    for (let candidate = lower; candidate <= upper; candidate += stride) compare(candidate);
  }
  const output = new Float32Array(hop);
  const tail = new Float32Array(hop);
  for (let i = 0; i < hop; i++) {
    const weight = overlap ? i / hop : Math.min(1, i / (rate * .005));
    output[i] = sample(start + i) * weight + (overlap?.[i] ?? 0) * (1 - weight);
    tail[i] = sample(start + hop + i);
  }
  return {output, tail};
}
