// @ts-check
import { defineConfig } from 'astro/config';
import starlight from '@astrojs/starlight';

export default defineConfig({
  site: 'https://arnav-kr.github.io',
  base: '/cmf-watch-utils',
  integrations: [
    starlight({
      title: 'CMF Watch Pro 2 Utils',
      description:
        'Reverse-engineered firmware, BLE, watch-face and OTA tooling for the CMF Watch Pro 2 (JX402 / ATS3089C).',
      social: [
        {
          icon: 'github',
          label: 'GitHub',
          href: 'https://github.com/arnav-kr/cmf-watch-utils',
        },
      ],
      editLink: {
        baseUrl: 'https://github.com/arnav-kr/cmf-watch-utils/edit/main/site/',
      },
      sidebar: [
        {
          label: 'Overview',
          items: [
            { label: 'What works', slug: 'overview/achievements' },
            { label: 'Limitations & risks', slug: 'overview/limitations' },
            { label: 'Methodology', slug: 'overview/methodology' },
            { label: 'Legal & attribution', slug: 'overview/legal' },
          ],
        },
        {
          label: 'Getting started',
          items: [
            { label: 'Requirements', slug: 'start/requirements' },
            { label: 'Setup', slug: 'start/setup' },
            { label: 'Quick start', slug: 'start/quickstart' },
          ],
        },
        {
          label: 'BLE & OTA',
          items: [
            { label: 'BLE protocol', slug: 'protocol/ble' },
            { label: 'AOTA OTA flow', slug: 'protocol/ota' },
            { label: 'Shell / AT channel', slug: 'protocol/shell' },
          ],
        },
        {
          label: 'Firmware formats',
          items: [
            { label: 'AOTA container', slug: 'formats/aota' },
            { label: 'SDFS filesystem', slug: 'formats/sdfs' },
            { label: 'Watch faces & WFDL', slug: 'formats/watchface' },
            { label: 'UI style bundles', slug: 'formats/style' },
            { label: 'Picture codecs', slug: 'formats/pictures' },
            { label: 'AJS animations', slug: 'formats/ajs' },
          ],
        },
        {
          label: 'Device internals',
          items: [
            { label: 'Hardware', slug: 'internals/hardware' },
            { label: 'app.bin internals', slug: 'internals/appbin' },
            { label: 'Recovery & boot modes', slug: 'internals/recovery' },
          ],
        },
        {
          label: 'Guides',
          items: [
            { label: 'Firmware OTA flash', slug: 'guides/ota-flash' },
            { label: 'Dial editing & upload', slug: 'guides/dial-editing' },
            { label: 'Bad Apple case study', slug: 'guides/bad-apple' },
            { label: 'Ghidra workflow', slug: 'guides/ghidra' },
          ],
        },
        {
          label: 'For AI agents',
          items: [
            { label: 'Agent context', slug: 'ai/context' },
            { label: 'Prompt recipes', slug: 'ai/prompts' },
          ],
        },
        {
          label: 'Dumps & data',
          items: [{ label: 'Data index', slug: 'dumps' }],
        },
      ],
    }),
  ],
});
