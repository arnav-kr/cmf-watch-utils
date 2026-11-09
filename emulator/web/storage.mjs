let database;

function openDatabase() {
  database ??= new Promise((resolve, reject) => {
    const request = indexedDB.open('cmf-watch-pro-2', 1);
    request.onupgradeneeded = () => request.result.createObjectStore('inputs');
    request.onsuccess = () => resolve(request.result);
    request.onerror = () => reject(request.error);
  });
  return database;
}

export async function savedInput(key, value) {
  const db = await openDatabase();
  const read = value === undefined;
  return new Promise((resolve, reject) => {
    const transaction = db.transaction('inputs', read ? 'readonly' : 'readwrite');
    const inputs = transaction.objectStore('inputs');
    const request = read ? inputs.get(key) : value ? inputs.put(value, key) : inputs.delete(key);
    transaction.oncomplete = () => resolve(request.result);
    transaction.onabort = () => reject(transaction.error);
  });
}
