const origin = 'https://fmc.freethinkel.dev';
const featuredIds = ['lew21snwfa2a16z', '84mhyziqu5559ap', 'vtn79ml3l3pllyi'];
const compatible = 'published=true && device="watch_pro_2"';

async function entries(filter, perPage) {
  const query = new URLSearchParams({
    filter, perPage, sort: '-created', expand: 'owner',
    fields: 'id,collectionId,name,preview,bin,expand.owner.name',
  });
  const response = await fetch(`${origin}/api/collections/watchfaces/records?${query}`, {
    signal: AbortSignal.timeout(12000),
  });
  if (!response.ok) throw Error('FMC could not load the watch-face catalog.');
  return (await response.json()).items.filter(face => face.name && face.preview && face.bin);
}

function fileUrl(face, filename) {
  const path = [face.collectionId, face.id, filename].map(encodeURIComponent).join('/');
  return `${origin}/api/files/${path}`;
}

export async function downloadCatalogFace(face) {
  const response = await fetch(fileUrl(face, face.bin), {signal: AbortSignal.timeout(15000)});
  if (!response.ok) throw Error('FMC could not download this watch face.');
  const file = await response.blob();
  if (!file.size || file.size > 3021359) throw Error('This watch face is outside the supported size.');
  return file;
}

export function setupFaceCatalog(picker) {
  const summary = picker.querySelector('summary');
  const choices = picker.querySelector('.catalog-options');
  const message = picker.querySelector('[role=status]');
  const clear = picker.querySelector('button');
  let selected;
  let loading = false;
  let loaded = false;

  function select(face) {
    selected = face;
    summary.textContent = face?.name || 'FMC watch faces';
    for (const button of choices.querySelectorAll('button')) {
      button.setAttribute('aria-pressed', String(button.value === face?.id));
    }
    picker.open = false;
    summary.focus();
  }
  clear.onclick = () => select(undefined);

  function addOption(face) {
    const item = document.createElement('li');
    const button = document.createElement('button');
    button.type = 'button';
    button.value = face.id;
    button.setAttribute('aria-pressed', 'false');
    const preview = document.createElement('img');
    preview.alt = '';
    preview.loading = 'lazy';
    preview.crossOrigin = 'anonymous';
    preview.src = fileUrl(face, face.preview);
    const text = document.createElement('span');
    const title = document.createElement('strong');
    title.textContent = face.name;
    const author = document.createElement('small');
    author.textContent = face.expand?.owner?.name || 'FMC creator';
    text.append(title, author);
    button.append(preview, text);
    button.onclick = () => select(face);
    item.append(button);
    choices.append(item);
  }

  picker.addEventListener('toggle', async () => {
    if (!picker.open || loading || loaded) return;
    loading = true;
    message.textContent = 'Loading FMC watch faces…';
    try {
      const ids = featuredIds.map(id => `id="${id}"`).join(' || ');
      const [featured, recent] = await Promise.all([
        entries(`${compatible} && (${ids})`, featuredIds.length),
        entries(compatible, 12),
      ]);
      const unique = new Map([...featured, ...recent].map(face => [face.id, face]));
      unique.forEach(addOption);
      loaded = true;
      message.textContent = unique.size ? '' : 'FMC has no Watch Pro 2 faces to show.';
    } catch {
      message.textContent = 'FMC could not load. Close and reopen the picker to retry.';
    } finally {
      loading = false;
    }
  });
  return () => selected;
}
