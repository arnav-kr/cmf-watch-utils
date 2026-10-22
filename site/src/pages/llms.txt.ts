import type { APIRoute } from 'astro';
import { getCollection } from 'astro:content';

export const prerender = true;

interface DocEntry {
  id: string;
  data: {
    title?: string;
    description?: string;
  };
  body?: string;
}

function urlFor(id: string, site: URL | undefined): string {
  const base = site ?? new URL('https://cmf.arnv.dev');
  const path = id === 'index' ? '/' : `/${id}/`;
  return new URL(path, base).href;
}

export const GET: APIRoute = async ({ site }) => {
  const docs = (await getCollection('docs')) as DocEntry[];
  const sorted = [...docs].sort((a, b) => a.id.localeCompare(b.id));

  const lines: string[] = [
    '# CMF Watch Utils',
    '',
    '> Tooling and documentation for CMF Watch and other smartwatches: AOTA/SDFS/WFDL/AJS firmware container',
    '> toolkit, BLE tooling, and per-device reverse-engineering notes. Unofficial project, not affiliated',
    '> with Nothing / CMF.',
    '',
    'The documentation is organised per device. Each device has its own path below.',
    '',
  ];

  for (const doc of sorted) {
    lines.push(`- [${doc.data.title ?? doc.id}](${urlFor(doc.id, site)}): ${doc.data.description ?? ''}`.trimEnd());
  }

  lines.push('');
  lines.push('## Optional',
    '',
    `- [Full text of all pages](${urlFor('llms-full', site)})`,
    `- [Repository](${urlFor('repo', site)})`.replace(urlFor('repo', site), 'https://github.com/arnav-kr/cmf-watch-utils'),
    `- [Author](https://github.com/arnav-kr) · [arnav.dev](https://arnv.dev)`,
    '');

  return new Response(lines.join('\n'), {
    headers: { 'Content-Type': 'text/plain; charset=utf-8' },
  });
};
