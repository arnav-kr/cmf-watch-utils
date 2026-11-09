import assert from 'node:assert/strict';
import {test} from 'node:test';
import {speakerHop, speakerSegment} from '../web/speaker.mjs';

const rate = 16000;
const hop = speakerHop(rate);
const tone = index => Math.sin(2 * Math.PI * 500 * index / rate);

test('a slower guest lengthens playback without lowering the note', () => {
  const speed = .56;
  let cursor = 0, tail;
  const output = [];
  while (cursor < rate * speed) {
    const segment = speakerSegment(tone, 0, rate, cursor, tail, rate, false);
    output.push(...segment.output);
    tail = segment.tail;
    cursor += hop * speed;
  }
  assert.ok(Math.abs(output.length - rate) <= hop);
  let crossings = 0;
  for (let i = hop + 1; i < output.length; i++) {
    if (output[i - 1] <= 0 && output[i] > 0) crossings++;
    assert.ok(Math.abs(output[i] - output[i - 1]) < .3);
  }
  const measuredPitch = crossings * rate / (output.length - hop);
  assert.ok(Math.abs(measuredPitch - 500) <= 5, `Measured ${measuredPitch} Hz`);
});

test('short sounds can flush without reading beyond the PCM file', () => {
  const end = 70;
  const read = index => {
    assert.ok(index >= 0 && index < end);
    return tone(index);
  };
  assert.equal(speakerSegment(read, 0, end, 0, undefined, rate, false), undefined);
  const {output} = speakerSegment(read, 0, end, 0, undefined, rate, true);
  assert.ok(output.some(sample => Math.abs(sample) > .1));
  assert.equal(output[output.length - 1], 0);
});
