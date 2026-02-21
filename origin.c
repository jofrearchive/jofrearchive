<!DOCTYPE html>
<html lang="es">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">
  <title>JOFRE ARCHIVE — Cloister Point Cloud</title>
  <script src="https://cdn.babylonjs.com/babylon.js"></script>
  <script src="https://cdn.babylonjs.com/loaders/babylonjs.loaders.min.js"></script>
  <script src="https://cdn.babylonjs.com/gui/babylon.gui.min.js"></script>
  <link href="https://fonts.googleapis.com/css2?family=EB+Garamond&family=Source+Code+Pro:wght@300;400&display=swap" rel="stylesheet">
  <style>
    :root {
      --bg-deep: #0b0b0b;
      --ivory: #e8e2d6;
      --led-green: #4ade80;
    }
    body, html { margin:0; padding:0; width:100%; height:100%; overflow:hidden; background: var(--bg-deep); }
    #renderCanvas { width:100%; height:100%; touch-action:none; filter: contrast(110%) brightness(90%); }

    /* Gate */
    #gate {
      position:absolute; inset:0;
      background: radial-gradient(circle at center, rgba(11,11,11,0.8) 0%, var(--bg-deep) 100%);
      display:flex; flex-direction:column; align-items:center; justify-content:center;
      z-index:10; color:var(--ivory); text-align:center;
    }
    .bab3d-on #gate{ display:none; }

    h1 {
      font-family:'EB Garamond', serif;
      font-weight: normal;
      letter-spacing: .2em;
      opacity:.9;
      margin: 0 0 40px 0;
    }

    #startVR {
      background: rgba(0,0,0,.6);
      border: 1px solid rgba(232,226,214,.2);
      color: var(--ivory);
      font-family:'Source Code Pro', monospace;
      text-transform: uppercase;
      letter-spacing: .18em;
      padding: 12px 30px;
      cursor:pointer;
      display:flex;
      align-items:center;
      gap: 15px;
      transition: all .3s ease;
    }
    #startVR:hover { border-color: rgba(232,226,214,.5); background: rgba(0,0,0,.8); }
    #led { width:8px; height:8px; background:#222; border-radius:50%; box-shadow: 0 0 0px var(--led-green); transition:.3s; }
    #startVR:hover #led { background: var(--led-green); box-shadow: 0 0 8px var(--led-green); }

    /* ===== PANEL PRINCIPAL ===== */
    #panel {
      position:fixed;
      left:50%;
      top:50%;
      transform: translate(-50%, -50%);
      width: min(480px, 88vw);
      background: rgba(10,10,10,.65);
      border:1px solid rgba(255,255,255,.18);
      /* PADDING UNIFORME SOLICITADO */
      padding: 30px !important; 
      backdrop-filter: blur(8px);
      z-index:8;
      opacity:0;
      transition: opacity 1.0s ease;
      display:none;
    }
    #panel.show{ opacity:1; display:block; }

    #panel #title{
      font-family:'EB Garamond', serif;
      font-size: clamp(28px, 4vw, 42px);
      letter-spacing: .18em;
      text-transform: uppercase;
      color: var(--ivory);
      margin: 0 0 16px 0;
    }
    #panel #submeta{
      font-family:'Source Code Pro', monospace;
      font-size: 11px;
      letter-spacing: .14em;
      text-transform: uppercase;
      opacity:.65;
      line-height: 1.7;
      margin-bottom: 18px;
      color: var(--ivory);
    }
    .tabs{ display:flex; gap: 12px; margin-bottom: 0; }
    .tabLink{
      flex: 1;
      background: rgba(255,255,255,.02);
      border:1px solid rgba(255,255,255,.16);
      color: var(--ivory);
      font-family:'Source Code Pro', monospace;
      font-size: 11px;
      letter-spacing: .12em;
      text-transform: uppercase;
      padding: 12px 10px;
      cursor:pointer;
      transition: all .2s ease;
      text-align: center;
    }
    .tabLink:hover, .tabLink.active{
      background: rgba(255,255,255,.08);
      border-color: rgba(255,255,255,.3);
      opacity: 1;
    }

    /* Ajuste para móviles */
    @media (max-width: 520px){
      #panel {
        width: 90vw !important;
        padding: 20px !important; /* Padding uniforme más pequeño en móvil */
      }
      #panel #title { font-size: 24px !important; }
      .tabLink { font-size: 10px !important; padding: 10px 5px !important; }
    }

    /* Status clases */
    body.bab3d-ready #panel{ display:none; }
    body.bab3d-on #gate{ display:none; }
    body.bab3d-on #panel{ display:block; }

    /* LOADER */
    #loader {
      position:fixed; inset:0;
      background: var(--bg-deep);
      z-index:9999;
      display:flex; align-items:center; justify-content:center;
    }
    #loader[aria-hidden="true"]{ display:none; }
    .loaderBox { width:min(760px, 92vw); border:1px solid rgba(255,255,255,.16); background: rgba(10,10,10,.60); padding: 30px; }
    .loaderTitle { font-family:'EB Garamond', serif; font-size: 20px; letter-spacing: .24em; text-transform: uppercase; color: var(--ivory); margin-bottom: 12px; }
    .typeLine { font-family: 'Source Code Pro', monospace; font-size: 14px; color: var(--ivory); min-height: 3em; opacity: 0.8; }
    .barWrap { margin-top: 18px; height: 4px; background: rgba(255,255,255,0.1); }
    .bar { height:100%; width:0%; background: var(--ivory); transition: width .1s; }
    .fadeOut { opacity:0; pointer-events:none; transition: opacity 1s ease; }

    #motionBtn{
      position: fixed; left: 50%; bottom: 30px; transform: translateX(-50%);
      z-index: 12; display: none; padding: 10px 20px;
      background: rgba(0,0,0,0.7); border: 1px solid var(--ivory);
      color: var(--ivory); font-family:'Source Code Pro', monospace; cursor: pointer;
    }
  </style>
</head>
<body class="bab3d-ready">

  <div id="gate">
    <h1>EXPERIMENTAL ARCHIVE</h1>
    <button id="startVR">
      <div id="led"></div>
      <span>Initialize System</span>
    </button>
  </div>

  <div id="loader" aria-hidden="true">
    <div class="loaderBox">
      <div class="loaderTitle">INITIALIZING SYSTEM</div>
      <div id="typeLine" class="typeLine">Boot sequence…</div>
      <div class="barWrap"><div id="bar" class="bar"></div></div>
    </div>
  </div>

  <canvas id="renderCanvas"></canvas>

  <button id="motionBtn">Enable Motion</button>

  <div id="panel">
    <div id="title">JOFRE OLIVERAS</div>
    <div id="submeta">ARTIST · EXPLORER · LANDSCAPER<br>EXPERIMENTAL ARCHIVE</div>
    <div class="tabs" id="tabs"></div>
  </div>

<script>
const PANEL_CONTENT = {
  tabs: [
    { key: "STUDIES", ctaHref: "https://inside.jofrearchive.com/studies/" },
    { key: "GENEALOGY", ctaHref: "https://inside.jofrearchive.com/genealogy/" },
    { key: "ARTIFACTS", ctaHref: "https://jofrearchive.com/artifacts" }
  ]
};

(function initPanel(){
  const tabsEl = document.getElementById("tabs");
  PANEL_CONTENT.tabs.forEach(t => {
    const b = document.createElement("button");
    b.className = "tabLink";
    b.textContent = t.key;
    b.onclick = () => { if(t.ctaHref) window.location.href = t.ctaHref; };
    tabsEl.appendChild(b);
  });

  const obs = new MutationObserver(() => {
    if (document.body.classList.contains("bab3d-on")) {
      document.getElementById("panel").classList.add("show");
    }
  });
  obs.observe(document.body, { attributes:true, attributeFilter:["class"] });
})();

(function initEngine(){
  const canvas = document.getElementById("renderCanvas");
  const engine = new BABYLON.Engine(canvas, true);
  let scene;

  async function createScene(){
    scene = new BABYLON.Scene(engine);
    scene.clearColor = new BABYLON.Color4(0.04, 0.04, 0.04, 1);

    // LUMINARIA
    new BABYLON.HemisphericLight("light", new BABYLON.Vector3(0, 1, 0), scene);

    // --- MODIFICACIÓN DE CÁMARA (MIRANDO HACIA ARRIBA) ---
    // Posición: x=0, y=-5 (abajo), z=0
    const camera = new BABYLON.FreeCamera("camera", new BABYLON.Vector3(0, -5, 0), scene);
    camera.setTarget(new BABYLON.Vector3(0, 0, 0)); // Mira al centro (arriba desde su posición)
    camera.attachControl(canvas, true);
    camera.inputs.removeByType("FreeCameraKeyboardMoveInput");

    if (/Android|iPhone|iPad/i.test(navigator.userAgent)) {
      camera.inputs.addDeviceOrientation();
    }

    // CARGA DE MODELO
    const modelUrl = "https://inside.jofrearchive.com/assets/3D/c.glb";
    try {
      const res = await BABYLON.SceneLoader.ImportMeshAsync("", "", modelUrl, scene);
      res.meshes.forEach(m => {
        if(m.material) {
          const pbr = new BABYLON.PBRMaterial("mat", scene);
          pbr.albedoColor = new BABYLON.Color3(0.1, 0.1, 0.1);
          m.material = pbr;
        }
      });
    } catch(e) { console.error("Error loading model", e); }

    return scene;
  }

  async function startSystem() {
    document.body.classList.remove("bab3d-ready");
    document.body.classList.add("bab3d-on");
    
    document.getElementById("loader").setAttribute("aria-hidden", "false");
    const bar = document.getElementById("bar");
    
    let p = 0;
    const int = setInterval(() => {
      p += (100 - p) * 0.1;
      bar.style.width = p + "%";
      if(p > 99) {
        clearInterval(int);
        setTimeout(() => document.getElementById("loader").classList.add("fadeOut"), 500);
      }
    }, 100);

    await createScene();
    engine.runRenderLoop(() => scene.render());
  }

  document.getElementById("startVR").addEventListener("click", startSystem);
  window.addEventListener("resize", () => engine.resize());
})();
</script>
</body>
</html>