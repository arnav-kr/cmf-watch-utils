const $ = (id) => document.getElementById(id);
const panel = $('panel');
const context = panel.getContext('2d', {alpha:false});
const pixels = context.createImageData(466,466);
let runtime, timer, pointer=null, crownDown=false, frames=0, lastFrame=null;
let runtimeLog='', halted=false, nextLog=0;
const debug=new URLSearchParams(location.search).has('debug');
const encoder = new TextEncoder();
function status(text,error=false,state) {
  $('status').textContent=text;
  $('status').toggleAttribute('data-error',error);
  if(error)state='error';
  if(state){$('state-dot').dataset.state=state;$('state-label').textContent=({running:'Running',loading:'Starting',error:'Stopped',ready:'Ready'})[state];}
}
function log(text) {
  runtimeLog=(runtimeLog+'\n'+text).slice(-8000);
  $('log').textContent=runtimeLog;
  if(/stopped on unimplemented|Unsupported|Aborted|Invalid WFC|Cannot read WFC/.test(text)){halted=true;status('Emulation stopped. See the boot log.',true);}
}
function input(line) {
  if (!runtime||halted) return;
  const bytes=encoder.encode(line+'\n');
  const stream=runtime.FS.open('/watch/input.txt','a');
  try {runtime.FS.write(stream,bytes,0,bytes.length);} finally {runtime.FS.close(stream);}
}
function crown(down) {
  if(crownDown===down)return;
  crownDown=down; input(`c ${down?1:0}`);
}
function point(event) {
  const rect=panel.getBoundingClientRect();
  let x=(event.clientX-rect.left)*466/rect.width,y=(event.clientY-rect.top)*466/rect.height;
  const dx=x-233,dy=y-233,r=Math.hypot(dx,dy);
  if(r>232){x=233+dx*232/r;y=233+dy*232/r;}
  return [Math.round(x),Math.round(y)];
}
function release() {
  if(pointer){input(`t 0 ${pointer.x} ${pointer.y}`);pointer=null;}
  crown(false);
}
panel.addEventListener('pointerdown',event=>{
  if(!runtime||pointer)return;
  const [x,y]=point(event);pointer={id:event.pointerId,x,y};
  panel.setPointerCapture(event.pointerId);panel.focus();input(`t 1 ${x} ${y}`);
});
panel.addEventListener('pointermove',event=>{
  if(!pointer||pointer.id!==event.pointerId)return;
  const [x,y]=point(event);
  if(x!==pointer.x||y!==pointer.y){Object.assign(pointer,{x,y});input(`t 1 ${x} ${y}`);}
});
for(const name of ['pointerup','pointercancel','lostpointercapture']) panel.addEventListener(name,release);
$('crown').addEventListener('pointerdown',event=>{event.preventDefault();$('crown').setPointerCapture(event.pointerId);crown(true);});
for(const name of ['pointerup','pointercancel','lostpointercapture']) $('crown').addEventListener(name,()=>crown(false));
$('up').onclick=()=>input('r 80');$('down').onclick=()=>input('r -80');
panel.addEventListener('wheel',event=>{if(runtime){event.preventDefault();input(`r ${event.deltaY>0?-80:80}`);}},{passive:false});
window.addEventListener('keydown',event=>{
  if(!runtime||halted||event.ctrlKey||event.altKey||event.metaKey||!['Space','ArrowUp','ArrowDown'].includes(event.code))return;
  if(event.target.closest('button,input,summary,a'))return;
  event.preventDefault();
  if(event.code==='Space')crown(true);
  else input(`r ${event.code==='ArrowUp'?80:-80}`);
});
window.addEventListener('keyup',event=>{if(event.code==='Space'){event.preventDefault();crown(false);}});
window.addEventListener('blur',release);
document.addEventListener('visibilitychange',()=>{if(document.hidden)release();});
function renderFrame() {
  try {
    const data=runtime.FS.readFile('/watch/frame.ppm');
    const header='P6\n466 466\n255\n';
    if(data.length!==header.length+466*466*3)return;
    for(let i=0;i<header.length;i++)if(data[i]!==header.charCodeAt(i))throw Error('Unexpected panel format');
    if(lastFrame&&data.every((v,i)=>v===lastFrame[i]))return;
    lastFrame=data;
    for(let i=0,j=header.length;i<pixels.data.length;i+=4,j+=3){
      pixels.data[i]=data[j];pixels.data[i+1]=data[j+1];pixels.data[i+2]=data[j+2];pixels.data[i+3]=255;
    }
    context.putImageData(pixels,0,0);frames++;
    $('frames').textContent=frames.toLocaleString();
    $('offline').hidden=true;
    if(!halted)status('Stock firmware is rendering locally.',false,'running');
  } catch(error) {if(error.name!=='ErrnoError'){log(error.message);clearInterval(timer);}}
}
function readLog(path,limit) {
  const length=runtime.FS.stat(path).size;
  const stream=runtime.FS.open(path,'r');
  try {
    const bytes=new Uint8Array(Math.min(length,limit));
    runtime.FS.read(stream,bytes,0,bytes.length,Math.max(0,length-limit));
    return new TextDecoder().decode(bytes);
  } finally {runtime.FS.close(stream);}
}
function render() {
  renderFrame();
  if(performance.now()<nextLog)return;
  nextLog=performance.now()+500;
  try {
    const uart=readLog('/watch/uart.log',32000).split('\n').filter(line=>!line.includes('#CD:')).join('\n');
    let diagnostic='';
    if(debug)try{diagnostic=readLog('/watch/qemu.log',12000);}catch{}
    $('log').textContent=runtimeLog+'\n'+diagnostic+'\n'+uart;
    if(uart.includes('ZEPHYR FATAL ERROR')){halted=true;status('Stock firmware stopped with a fatal error. See the boot log.',true);}
  } catch {}
}
$('show-frame').onchange=event=>$('case').classList.toggle('frameless',!event.target.checked);
$('fullscreen').onclick=async()=>{
  try {
    if(document.fullscreenElement)await document.exitFullscreen();
    else await document.querySelector('.device-card').requestFullscreen();
  }catch(error){log(error.message);}
};
document.addEventListener('fullscreenchange',()=>{
  $('fullscreen').setAttribute('aria-label',document.fullscreenElement?'Exit expanded watch view':'Expand watch view');
});
$('save-log').onclick=()=>{
  const url=URL.createObjectURL(new Blob([$('log').textContent],{type:'text/plain'}));
  const link=document.createElement('a');link.href=url;link.download='wp2-browser.log';link.click();
  setTimeout(()=>URL.revokeObjectURL(url),1000);
};
$('stop').onclick=()=>location.reload();
$('start').onclick=async()=>{
  $('start').disabled=true;
  try {
    if(!crossOriginIsolated)throw Error('Cross-origin isolation is required. Start the supplied local server.');
    status('Loading the runtime and stock storage...',false,'loading');
    $('stop').disabled=false;
    const response=await fetch('./fixtures/manifest.json');
    if(!response.ok)throw Error('Prepare the local fixture manifest first.');
    const manifest=await response.json();
    const files=await Promise.all(manifest.files.map(async name=>{
      const r=await fetch(`./fixtures/${name}`);if(!r.ok)throw Error(`Missing ${name}`);
      return [name,new Uint8Array(await r.arrayBuffer())];
    }));
    const {default:createQemu}=await import('./runtime/qemu-system-arm.js');
    runtime=await createQemu({
      mainScriptUrlOrBlob:new URL('./runtime/qemu-system-arm.js',location.href).href,
      locateFile:name=>new URL(`./runtime/${name}`,location.href).href,
      print:log,printErr:log,onAbort:reason=>{halted=true;status(`Runtime aborted: ${reason}`,true);},
      onExit:code=>{halted=true;status(`Emulator exited (${code}). See the boot log.`,true);},
      arguments:['-M','ats3089c-wp2','-bios','/watch/compat.bin','-kernel','/watch/app.bin',
        ...(debug?['-d','unimp','-D','/watch/qemu.log']:[]),
        '-display','none','-monitor','none','-serial','file:/watch/uart.log','-serial','null','-serial','null',
        '-icount','shift=3,align=off','-no-reboot'],
      preRun:[module=>{
        module.FS.mkdir('/watch');
        for(const [name,data]of files)module.FS.writeFile(`/watch/${name}`,data);
        module.FS.writeFile('/watch/input.txt','');
        Object.assign(module.ENV,{QEMU_WP2_NOR:'/watch/nor.bin',QEMU_WP2_NAND:'/watch/nand-defaults.bin',
          QEMU_WP2_FRAME:'/watch/frame.ppm',QEMU_WP2_INPUT:'/watch/input.txt',QEMU_WP2_LIVE_FRAME:'1',
          QEMU_WP2_LIVE_SENSORS:'1',QEMU_WP2_LIVE_HEALTH:'1',QEMU_WP2_STOCK_AWAKE:'1',QEMU_WP2_HOST_RTC:'1',QEMU_WP2_UNPLUGGED:'1'});
        if(manifest.files.includes('wfc-index.txt')&&!new URLSearchParams(location.search).has('no-textures'))module.ENV.QEMU_WP2_WFC_INDEX='/watch/wfc-index.txt';
      }]
    });
    for(const id of ['stop','crown','up','down'])$(id).disabled=false;
    status('Waiting for the stock display...',false,'loading');timer=setInterval(render,50);
  }catch(error){status(error.message,true);log(error.stack);}
};

