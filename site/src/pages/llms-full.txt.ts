import type { APIRoute } from 'astro';
import { getCollection, render } from 'astro:content';

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

  const parts: string[] = [
    '# CMF Watch Utils',
    '',
    'Full text of the documentation. Unofficial project, not affiliated with Nothing / CMF.',
    '',
  ];

  for (const doc of sorted) {
    let body = doc.body ?? '';
    try {
      const rendered = await render(doc as never);
      body = rendered.Content ? stripHtml(rendered.Content) : body;
    } catch {
      body = doc.body ?? '';
    }
    parts.push(`# ${doc.data.title ?? doc.id}`);
    parts.push('');
    if (doc.data.description) {
      parts.push(doc.data.description);
      parts.push('');
    }
    parts.push(`URL: ${urlFor(doc.id, site)}`);
    parts.push('');
    parts.push(body.trim());
    parts.push('');
    parts.push('---');
    parts.push('');
  }

  return new Response(parts.join('\n'), {
    headers: { 'Content-Type': 'text/plain; charset=utf-8' },
  });
};

function stripHtml(html: string): string {
  return html
    .replace(/<script[\s\S]*?<\/script>/gi, '')
    .replace(/<style[\s\S]*?<\/style>/gi, '')
    .replace(/<[^>]+>/g, ' ')
    .replace(/&lt;/g, '<')
    .replace(/&gt;/g, '>')
    .replace(/&amp;/g, '&')
    .replace(/&quot;/g, '"')
    .replace(/&#39;/g, "'")
    .replace(/[ \t]+/g, ' ')
    .replace(/\n{3,}/g, '\n\n')
    .trim();
}
