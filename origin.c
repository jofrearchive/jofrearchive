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

    /* ===== PANEL CONFIGURACIÓN ===== */
    #panel {
      position:fixed;
      left:50%;
      top:50%;
      transform: translate(-50%, -50%);
      width: min(520px, 86vw);
      background: rgba(10,10,10,.65);
      border:1px solid rgba(255,255,255,.18);
      backdrop-filter: blur(8px);
      z-index:8;
      opacity:0;
      transition: opacity 1.0s ease;
      display:none;
      box-sizing: border-box;
      
      /* MODIFICACIÓN: Margen interno igual en todos los lados para Desktop */
      padding: 45px !important; 
    }
    #panel.show { opacity:1; display:block; }

    #panel #title {
      font-family:'EB Garamond', serif;
      font-size: 32px;
      letter-spacing: .16em;
      text-transform: uppercase;
      color: var(--ivory);
      margin: 0 0 20px 0;
    }
    #panel #submeta {
      font-family:'Source Code Pro', monospace;
      font-size: 11px;
      letter-spacing: .14em;
      text-transform: uppercase;
      opacity:.65;
      line-height:1.7;
      margin-bottom: 25px;
      color: var(--ivory);
    }
    .tabs { 
      display:flex; 
      flex-direction: row; 
      gap: 15px; 
      margin-top: 20px;
    }
    .tabLink {
      flex: 1;
      background: rgba(255,255,255,.02);
      border:1px solid rgba(255,255,255,.16);
      color: var(--ivory);
      padding: 15px 10px;
      cursor:pointer;
      font-family:'Source Code Pro', monospace;
      font-size: 12px;
      transition: all .3s ease;
      text-align: center;
    }
    .tabLink:hover {
      background: rgba(255,255,255,.05);
      border-color: rgba(255,255,255,.3);
    }

    /* Versión Móvil: Reducimos el padding para que no ocupe toda la pantalla */
    @media (max-width: 520px) {
      #panel {
        padding: 20px !important;
        width: 92vw !important;
      }
      .tabs { gap: 8px; }
      .tabLink { padding: 10px 5px; font-size: 10px; }
    }

    /* LOADER */
    #loader {
      position:fixed; inset:0;
      background: var(--bg-deep);
      z-index:9999;
      display:flex; align-items:center; justify-content:center;
    }
    #loader[aria-hidden="true"] { display:none; }
    .loaderBox { width:min(700px, 90vw); border:1px solid rgba(255,255,255,.1); padding: 40px; background: rgba(0,0,0,0.4); }
    .barWrap { margin-top: 20px; height: 2px; background: rgba(255,255,255,0.1); }
    .bar { height:100%; width:0%; background: var(--ivory); transition: width 0.1s linear; }
    .fadeOut { opacity:0; pointer-events:none; transition: opacity 1s ease; }

    body.bab3d-on #gate { display:none; }
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
      <div id="typeLine" style="color:var(--ivory); font-family:'Source Code Pro'; margin-bottom:10px;">Boot sequence…</div>
      <div class="barWrap"><div id="bar" class="bar"></div></div>
    </div>
  </div>

  <canvas id="renderCanvas"></canvas>

  <div id="panel">
    <div id="title">JOFRE OLIVERAS</div>
    <div id="submeta">ARTIST · EXPLORER · LANDSCAPER<br>EXPERIMENTAL ARCHIVE</div>
    <div class="tabs" id="tabs"></div>
  </div>

<script>
const PANEL_CONTENT = {
  tabs: [
    { key: "STUDIES", href: "https://inside.jofrearchive.com/studies/" },
    { key: "GENEALOGY", href: "https://inside.jofrearchive.com/genealogy/" },
    { key: "ARTIFACTS", href: "https://jofrearchive.com/artifacts" }
  ]
};

(function initPanel(){
  const tabsEl = document.getElementById("tabs");
  PANEL_CONTENT.tabs.forEach(t => {
    const b = document.createElement("button");
    b.className = "tabLink";
    b.textContent = t.key;
    b.onclick = () => window.location.href = t.href;
    tabsEl.appendChild(b);
  });
  const obs = new MutationObserver(() => {
    if (document.body.classList.contains("bab3d-on")) document.getElementById("panel").classList.add("show");
  });
  obs.observe(document.body, { attributes:true, attributeFilter:["class"] });
})();

(function initEngine(){
  const canvas = document.getElementById("renderCanvas");
  const engine = new BABYLON.Engine(canvas, true);
  let scene;

  async function createScene(){
    scene = new BABYLON.Scene(engine);
    scene.clearColor = new BABYLON.Color4(0.043, 0.043, 0.043, 1);

    // Luces
    new BABYLON.HemisphericLight("light", new BABYLON.Vector3(0, 1, 0), scene).intensity = 0.7;

    // --- MODIFICACIÓN POSICIÓN CÁMARA ---
    // Colocamos la cámara abajo (Y = -6) y centrada.
    const camera = new BABYLON.FreeCamera("camera", new BABYLON.Vector3(0, -6, 0), scene);
    // Hacemos que mire hacia arriba (Y positiva)
    camera.setTarget(new BABYLON.Vector3(0, 5, 0));
    camera.attachControl(canvas, true);
    camera.inputs.removeByType("FreeCameraKeyboardMoveInput");

    // Carga del modelo
    const modelUrl = "https://inside.jofrearchive.com/assets/3D/c.glb";
    try {
      const res = await BABYLON.SceneLoader.ImportMeshAsync("", "", modelUrl, scene);
      res.meshes.forEach(m => {
        if(m.getTotalVertices() > 0) {
          m.material = new BABYLON.PBRMaterial("m", scene);
          m.material.albedoColor = new BABYLON.Color3(0.1, 0.1, 0.1);
        }
      });
    } catch(e) { console.error(e); }

    return scene;
  }

  async function start() {
    document.body.classList.add("bab3d-on");
    const loader = document.getElementById("loader");
    loader.setAttribute("aria-hidden", "false");
    
    let p = 0;
    const bar = document.getElementById("bar");
    const interval = setInterval(() => {
      p += (100 - p) * 0.1;
      bar.style.width = p + "%";
      if(p > 98) {
        clearInterval(interval);
        setTimeout(() => loader.classList.add("fadeOut"), 600);
      }
    }, 80);

    await createScene();
    engine.runRenderLoop(() => scene.render());
  }

  document.getElementById("startVR").addEventListener("click", start);
  window.addEventListener("resize", () => engine.resize());
})();
</script>
</body>
</html>