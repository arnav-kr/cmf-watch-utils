// @ts-check
import { defineConfig } from 'astro/config';
import starlight from '@astrojs/starlight';

export default defineConfig({
  site: 'https://arnav.is-a.dev',
  base: '/watch-utils',
  integrations: [
    starlight({
      title: 'Watch Utils',
      description:
        'Tooling and documentation for CMF Watch and other smartwatches: firmware containers, BLE tooling and per-device research notes.',
      social: [
        {
          icon: 'github',
          label: 'GitHub',
          href: 'https://github.com/arnav-kr/watch-utils',
        },
      ],
      editLink: {
        baseUrl: 'https://github.com/arnav-kr/watch-utils/edit/main/site/',
      },
      components: {
        Header: './src/components/Header.astro',
      },
      sidebar: [
        {
          label: 'About',
          items: [
            { label: 'Roadmap & devices', slug: 'about/roadmap' },
            { label: 'Methodology', slug: 'about/methodology' },
            { label: 'Legal & attribution', slug: 'about/legal' },
          ],
        },
        {
          label: 'Getting started',
          items: [
            { label: 'Requirements', slug: 'getting-started/requirements' },
            { label: 'Setup', slug: 'getting-started/setup' },
            { label: 'Quick start', slug: 'getting-started/quickstart' },
          ],
        },
        {
          label: 'Tooling',
          items: [
            { label: 'fwkit CLI', slug: 'tooling/fwkit' },
            { label: 'BLE tools', slug: 'tooling/ble-tools' },
          ],
        },
        {
          label: 'Devices',
          items: [
            {
              label: 'CMF Watch Pro 2',
              items: [
                { label: 'Device profile', slug: 'devices/cmf-watch-pro-2' },
                { label: 'Verified status', slug: 'devices/cmf-watch-pro-2/status' },
                { label: 'Limitations & risks', slug: 'devices/cmf-watch-pro-2/limitations' },
                { label: 'Protocol', slug: 'devices/cmf-watch-pro-2/protocol/ble' },
                { label: 'AOTA OTA flow', slug: 'devices/cmf-watch-pro-2/protocol/ota' },
                { label: 'Shell / AT channel', slug: 'devices/cmf-watch-pro-2/protocol/shell' },
                { label: 'AOTA container', slug: 'devices/cmf-watch-pro-2/formats/aota' },
                { label: 'SDFS filesystem', slug: 'devices/cmf-watch-pro-2/formats/sdfs' },
                { label: 'Watch faces & WFDL', slug: 'devices/cmf-watch-pro-2/formats/watchface' },
                { label: 'UI style bundles', slug: 'devices/cmf-watch-pro-2/formats/style' },
                { label: 'Picture codecs', slug: 'devices/cmf-watch-pro-2/formats/pictures' },
                { label: 'AJS animations', slug: 'devices/cmf-watch-pro-2/formats/ajs' },
                { label: 'Hardware', slug: 'devices/cmf-watch-pro-2/internals/hardware' },
                { label: 'app.bin internals', slug: 'devices/cmf-watch-pro-2/internals/appbin' },
                { label: 'Recovery & boot modes', slug: 'devices/cmf-watch-pro-2/internals/recovery' },
                { label: 'Firmware OTA flash', slug: 'devices/cmf-watch-pro-2/guides/ota-flash' },
                { label: 'Dial editing & upload', slug: 'devices/cmf-watch-pro-2/guides/dial-editing' },
                { label: 'Bad Apple case study', slug: 'devices/cmf-watch-pro-2/guides/bad-apple' },
                { label: 'Ghidra workflow', slug: 'devices/cmf-watch-pro-2/guides/ghidra' },
              ],
            },
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
