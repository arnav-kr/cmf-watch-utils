self.onmessage = async ({data: firmware}) => {
  try {
    const indexURL = 'https://cdn.jsdelivr.net/pyodide/v0.27.7/full/';
    importScripts(indexURL + 'pyodide.js');
    const python = await loadPyodide({indexURL});
    await python.loadPackage('lzma');
    const source = await fetch('./firmware-tools.zip');
    if (!source.ok) throw Error('The firmware validator could not be downloaded.');
    python.unpackArchive(await source.arrayBuffer(), 'zip', {extractDir: '/firmware-tools'});
    python.FS.writeFile('/update.bin', new Uint8Array(firmware));
    python.runPython(`
import sys
from pathlib import Path
sys.path.insert(0, "/firmware-tools")
from cmf_watch_utils.emu.image import verify_package
from cmf_watch_utils.emu.profiles import WP2
app, resources, _ = verify_package(Path("/update.bin").read_bytes(), WP2)
Path("/app.bin").write_bytes(app)
Path("/sdfs.bin").write_bytes(resources)
`);
    const app = python.FS.readFile('/app.bin');
    const sdfs = python.FS.readFile('/sdfs.bin');
    self.postMessage({app, sdfs}, [app.buffer, sdfs.buffer]);
  } catch (error) {
    self.postMessage({error: error.message.trim().split('\n').at(-1)});
  }
};
