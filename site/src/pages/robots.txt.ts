import type { APIRoute } from 'astro';

export const prerender = true;

export const GET: APIRoute = ({ site }) => {
  const base = site ?? new URL('https://cmf.arnv.dev');
  const lines = [
    'User-agent: *',
    'Allow: /',
    '',
    `Sitemap: ${new URL('/sitemap-index.xml', base).href}`,
    `Sitemap: ${new URL('/sitemap-0.xml', base).href}`,
    '',
    '# AI-readable entry points',
    `Allow: /llms.txt`,
    `Allow: /llms-full.txt`,
    '',
  ];
  return new Response(lines.join('\n'), {
    headers: { 'Content-Type': 'text/plain; charset=utf-8' },
  });
};
