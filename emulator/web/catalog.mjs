const origin = 'https://fmc.freethinkel.dev';
const compatible = 'published=true && (device="watch_pro_2" || device="")';

async function entries() {
  const faces = new Map();
  const query = new URLSearchParams({
    filter: compatible, perPage: 100, sort: '-created,id', expand: 'owner',
    fields: 'id,collectionId,name,preview,bin,downloads,flashes,expand.owner.name',
  });
  let totalPages;
  for (let page = 1; totalPages === undefined || page <= totalPages; page++) {
    query.set('page', page);
    const response = await fetch(`${origin}/api/collections/watchfaces/records?${query}`, {
      signal: AbortSignal.timeout(12000),
    });
    if (!response.ok) throw Error('FMC could not load the watch-face catalog.');
    const data = await response.json();
    if (!Number.isInteger(data.totalPages) || !Array.isArray(data.items)) throw Error('Invalid FMC response.');
    totalPages = data.totalPages;
    for (const face of data.items) {
      if (face.name && face.bin) faces.set(face.id, face);
    }
  }
  return [...faces.values()];
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

export function setupFaceCatalog(picker, initial, onApply) {
  const choices = picker.querySelector('.catalog-options');
  const message = picker.querySelector('[role=status]');
  const clear = picker.querySelector('.catalog-clear');
  const apply = picker.querySelector('.catalog-apply');
  const selection = picker.querySelector('#catalog-selection');
  const search = picker.querySelector('input[type=search]');
  const sort = picker.querySelector('select');
  const count = new Intl.NumberFormat();
  let faces;
  let selected = initial;
  let applied;
  let loading = false;

  function updateSelection() {
    const changed = selected?.id !== applied?.id;
    clear.disabled = !selected;
    apply.disabled = !changed;
    selection.textContent = changed ? `Selected: ${selected?.name || 'Stock face'}`
      : (applied ? `Applied: ${applied.name}` : 'No FMC face selected');
    for (const button of choices.querySelectorAll('button')) {
      button.setAttribute('aria-pressed', String(button.value === selected?.id));
    }
  }
  function select(face) {
    selected = face;
    updateSelection();
  }
  clear.onclick = () => select(undefined);
  apply.onclick = () => onApply(selected);
  updateSelection();

  function addOption(face) {
    const item = document.createElement('li');
    const button = document.createElement('button');
    button.type = 'button';
    button.value = face.id;
    button.setAttribute('aria-pressed', 'false');
    const preview = document.createElement('span');
    preview.className = 'catalog-preview';
    if (face.preview) {
      const image = document.createElement('img');
      image.alt = '';
      image.loading = 'lazy';
      image.crossOrigin = 'anonymous';
      image.src = fileUrl(face, face.preview);
      preview.append(image);
    }
    const text = document.createElement('span');
    const title = document.createElement('strong');
    title.textContent = face.name;
    const author = document.createElement('small');
    author.textContent = face.expand?.owner?.name || 'FMC creator';
    const activity = document.createElement('small');
    activity.textContent = `${count.format(face.downloads || 0)} downloads · ${count.format(face.flashes || 0)} flashes`;
    text.append(title, author, activity);
    button.setAttribute('aria-pressed', String(face.id === selected?.id));
    button.append(preview, text);
    button.onclick = () => select(face);
    item.append(button);
    choices.append(item);
  }

  function showResults() {
    if (!faces) return;
    const terms = search.value.trim().toLocaleLowerCase().split(/\s+/).filter(Boolean);
    const results = faces.filter(face => {
      const text = `${face.name} ${face.expand?.owner?.name || ''}`.toLocaleLowerCase();
      return terms.every(term => text.includes(term));
    });
    if (sort.value !== 'newest') {
      const field = sort.value;
      results.sort((a, b) => (b[field] || 0) - (a[field] || 0));
    }
    choices.replaceChildren();
    results.forEach(addOption);
    choices.scrollTop = 0;
    message.textContent = results.length
      ? `${count.format(results.length)} ${results.length === 1 ? 'watch face' : 'watch faces'}`
      : (terms.length ? 'No watch faces match your search.' : 'FMC has no Watch Pro 2 faces to show.');
  }
  search.addEventListener('input', showResults);
  sort.addEventListener('change', showResults);

  picker.addEventListener('toggle', async () => {
    if (!picker.open || loading || faces) return;
    loading = true;
    message.textContent = 'Loading FMC watch faces…';
    try {
      faces = await entries();
      showResults();
    } catch {
      message.textContent = 'FMC could not load. Close and reopen the picker to retry.';
    } finally {
      loading = false;
    }
  });
  return {
    selected: () => selected,
    markApplied(face) {
      applied = face;
      updateSelection();
    },
  };
}
