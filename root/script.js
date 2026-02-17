// ====== VITRINE BASIC SCRIPT ======
// Placeholder canvas animation (replace with Three.js / your engine)

const canvas = document.getElementById('scene');
const ctx = canvas.getContext('2d');

function resize(){
  const dpr = window.devicePixelRatio || 1;
  canvas.width = Math.floor(canvas.clientWidth * dpr);
  canvas.height = Math.floor(canvas.clientHeight * dpr);
  ctx.setTransform(dpr,0,0,dpr,0,0);
}
window.addEventListener('resize', resize);
resize();

let t = 0;
function draw(){
  ctx.clearRect(0,0,canvas.width,canvas.height);
  const w = canvas.clientWidth, h = canvas.clientHeight;
  ctx.strokeStyle = 'rgba(232,226,214,.35)';
  ctx.lineWidth = 1;
  ctx.beginPath();
  for(let x=0; x<w; x+=8){
    const y = h/2 + Math.sin((x+t)/40)*40;
    ctx.lineTo(x,y);
  }
  ctx.stroke();
  t += 1;
  requestAnimationFrame(draw);
}
draw();
