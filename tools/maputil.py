import json

_TEMPLATE = """<!doctype html>
<html><head><meta charset="utf-8"><title>__TITLE__</title>
<meta name="viewport" content="width=device-width,initial-scale=1">
<link rel="stylesheet" href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css"/>
<style>
html,body,#map{height:100%;margin:0;background:#111}
.legend,.pal{background:#fff;padding:8px 10px;border-radius:6px;line-height:1.5em;
  font:12px/1.5 system-ui,sans-serif;color:#111}
.legend i{display:inline-block;width:110px;height:10px;vertical-align:middle;margin-right:6px}
.pal select{font:12px system-ui,sans-serif}
</style></head>
<body><div id="map"></div>
<script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js"></script>
<script>
const D = __DATA__;
const PALETTES = {
  'Blue-Red':   [[59,76,192],[145,191,219],[221,221,221],[244,154,123],[180,4,38]],
  'Turbo':      [[48,18,59],[62,155,254],[24,214,186],[164,252,60],[254,138,24],[122,4,3]],
  'Viridis':    [[68,1,84],[59,82,139],[33,145,140],[94,201,98],[253,231,37]],
  'Plasma':     [[13,8,135],[126,3,168],[204,71,120],[248,149,64],[240,249,33]],
  'Inferno':    [[0,0,4],[87,16,110],[188,55,84],[249,142,9],[252,255,164]],
  'Mono':       [[230,57,70]]
};
const ZONES = [[59,76,192],[110,168,255],[125,223,100],[255,209,102],[230,57,70]];
const NOHR = '#9aa0a6';

const map = L.map('map');
const baseLayers = {
  'Esri Satellite': L.tileLayer('https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}',
    {maxZoom:19, attribution:'Tiles &copy; Esri'}),
  'Carto Light': L.tileLayer('https://{s}.basemaps.cartocdn.com/light_all/{z}/{x}/{y}{r}.png',
    {maxZoom:19, subdomains:'abcd', attribution:'&copy; OSM &copy; CARTO'}),
  'OpenTopoMap': L.tileLayer('https://{s}.tile.opentopomap.org/{z}/{x}/{y}.png',
    {maxZoom:17, subdomains:'abc', attribution:'&copy; OpenTopoMap'})
};
baseLayers['Esri Satellite'].addTo(map);

function rgb(c){ return 'rgb(' + c.map(v => Math.round(v)).join(',') + ')'; }
function fracOf(hr){ return D.vmax > D.vmin ? (hr - D.vmin) / (D.vmax - D.vmin) : 0.5; }
function colorOf(pal, frac){
  frac = Math.min(Math.max(frac, 0), 1);
  if (pal === 'Zones') return rgb(ZONES[Math.min(ZONES.length - 1, Math.floor(frac * ZONES.length))]);
  const st = PALETTES[pal];
  const x = frac * (st.length - 1);
  const i = Math.min(st.length - 2, Math.floor(x));
  const t = x - i, a = st[i], b = st[i + 1];
  return rgb([a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t]);
}

let palette = 'Blue-Red';

function fillGroup(grp, l0, pal){
  grp.clearLayers();
  let cur = [], curcol = null, prev = null;
  const flush = () => { if (cur.length > 1) L.polyline(cur, {color: curcol, weight: 5, opacity: 0.9}).addTo(grp); cur = []; };
  l0.pts.forEach(p => {
    const ll = [p[0], p[1]];
    const col = (p[2] == null) ? NOHR : colorOf(pal, fracOf(p[2]));
    if (col !== curcol) { flush(); curcol = col; cur = prev ? [prev, ll] : [ll]; }
    else cur.push(ll);
    prev = ll;
  });
  flush();
  if (l0.pts.length) {
    L.circleMarker(l0.pts[0], {color:'#2a9d8f', weight:2, radius:6, fillOpacity:1, fillColor:'#2a9d8f'})
      .bindPopup('<b>' + l0.label + '</b><br>' + l0.info).addTo(grp);
    L.circleMarker(l0.pts[l0.pts.length-1], {color:'#1d3557', weight:2, radius:6, fillOpacity:1, fillColor:'#1d3557'}).addTo(grp);
  }
}

const state = D.layers.map(l0 => ({l0, grp: L.featureGroup()}));
const overlays = {};
const all = [];
state.forEach((s, i) => {
  const name = s.l0.label || ('Session ' + (i + 1));
  overlays[name] = s.grp;
  fillGroup(s.grp, s.l0, palette);
  s.l0.pts.forEach(p => all.push([p[0], p[1]]));
  if (D.layers.length <= 6 || i === D.layers.length - 1) s.grp.addTo(map);
});
L.control.layers(baseLayers, overlays, {collapsed:false}).addTo(map);
if (all.length) map.fitBounds(L.latLngBounds(all).pad(0.08));

const legend = L.control({position:'bottomright'});
legend.onAdd = () => {
  const d = L.DomUtil.create('div', 'legend');
  d.id = 'legend';
  return d;
};
legend.addTo(map);

const palCtl = L.control({position:'topright'});
palCtl.onAdd = () => {
  const d = L.DomUtil.create('div', 'pal');
  d.innerHTML = '<b>Color</b> <select>' +
    ['Blue-Red','Turbo','Viridis','Plasma','Inferno','Zones','Mono']
      .map(p => '<option>' + p + '</option>').join('') + '</select>';
  L.DomEvent.disableClickPropagation(d);
  d.querySelector('select').onchange = e => { palette = e.target.value; repaint(); };
  return d;
};
palCtl.addTo(map);

function repaint(){
  state.forEach(s => fillGroup(s.grp, s.l0, palette));
  const el = document.getElementById('legend');
  if (!el) return;
  const c = colorOf(palette, 0), c2 = colorOf(palette, 1), cm = colorOf(palette, 0.5);
  const grad = 'linear-gradient(90deg,' + c + ',' + cm + ',' + c2 + ')';
  el.innerHTML = '<div><b>Heart rate</b></div>' +
    '<div><i style="background:' + c + '"></i>' + (D.vmax ? D.vmin + ' bpm' : 'n/a') + '</div>' +
    '<div><i style="background:' + grad + '"></i></div>' +
    '<div><i style="background:' + c2 + '"></i>' + (D.vmax ? D.vmax + ' bpm' : '') + '</div>' +
    '<div><i style="background:' + NOHR + '"></i>no HR</div>';
}
repaint();
</script></body></html>"""


def _dump(layers, vmin, vmax, out, title):
    html = _TEMPLATE.replace("__DATA__", json.dumps({"vmin": vmin, "vmax": vmax, "layers": layers}))
    html = html.replace("__TITLE__", title)
    with open(out, "w", encoding="utf-8") as f:
        f.write(html)
    return out


def write_map(points, out="gps_map.html", title="GPS track"):
    pts = [[round(p[0], 6), round(p[1], 6), None] for p in points]
    return _dump([{"label": title, "info": f"{len(pts)} points", "pts": pts}], 0, 0, out, title)


def write_sessions_map(sessions, out="gps_map.html", title="GPS sessions"):
    hrs = [p[2] for s in sessions for p in s["points"] if p[2] is not None]
    vmin, vmax = (min(hrs), max(hrs)) if hrs else (0, 0)
    layers = [{"label": s["label"], "info": s.get("info", ""),
               "pts": [[round(la, 6), round(lo, 6), hr] for la, lo, hr in s["points"]]}
              for s in sessions]
    return _dump(layers, vmin, vmax, out, title)
