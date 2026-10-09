#pragma once
#include <Arduino.h>

const char PAGINA[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Monitor Ambiental</title>
<style>
:root{--bg:#F4F7FB;--surface:#FFFFFF;--border:#DCE5F0;--text:#0F172A;--muted:#5B6B82;--grid:#E5EAF2;
--accent:#1D4ED8;--accent2:#0EA5E9;--g1:#0B2545;--g2:#13315C;--g3:#1D4ED8;--on:#FFFFFF;
--ok:#10B981;--warn:#F59E0B;--danger:#EF4444}
*{box-sizing:border-box}
body{margin:0;background:var(--bg);color:var(--text);font-family:system-ui,-apple-system,"Segoe UI",Roboto,sans-serif}
.wrap{max-width:1100px;margin:0 auto;padding:14px}
.banner{display:flex;justify-content:space-between;align-items:center;gap:12px;flex-wrap:wrap;
background:linear-gradient(135deg,var(--g1),var(--g2) 55%,var(--g3));border-radius:14px;padding:18px 20px;color:#fff;margin-bottom:14px}
.eyebrow{font-size:.68rem;letter-spacing:.14em;font-weight:700;opacity:.7}
.titulo{font-size:1.45rem;font-weight:700;margin:2px 0}
.sub{font-size:.85rem;opacity:.8}
.dir{text-align:right}
.relogio{font-size:1.7rem;font-weight:700;font-variant-numeric:tabular-nums;margin-top:6px;line-height:1.1}
.data{font-size:.78rem;opacity:.8}
.badge{display:inline-block;padding:5px 12px;border-radius:999px;font-size:.7rem;font-weight:700;letter-spacing:.06em}
.b-ok{background:#D1FAE5;color:#047857}.b-aviso{background:#FEF3C7;color:#B45309}.b-erro{background:#FEE2E2;color:#B91C1C}
.chips{display:flex;gap:8px;flex-wrap:wrap;margin-bottom:14px}
.chip{display:inline-flex;align-items:center;gap:6px;background:var(--surface);color:var(--text);border:1px solid var(--border);border-radius:999px;padding:7px 14px;font-size:.8rem;font-weight:700;cursor:pointer}
.chip.on{background:var(--accent);color:var(--on);border-color:var(--accent)}
.pt{width:9px;height:9px;border-radius:50%;display:inline-block}
.alerta{border:1px solid;border-left-width:4px;border-radius:10px;padding:10px 14px;margin-bottom:14px;font-size:.9rem}
.alerta.ok{background:#10B98114;border-color:#10B98155;border-left-color:var(--ok)}
.alerta.erro{background:#EF444416;border-color:#EF444455;border-left-color:var(--danger)}
.alerta.info{background:#94A3B81A;border-color:var(--border);border-left-color:var(--accent)}
.cards{display:grid;grid-template-columns:repeat(auto-fit,minmax(150px,1fr));gap:12px;margin-bottom:12px}
.card{background:var(--surface);border:1px solid var(--border);border-left:4px solid var(--c,var(--accent));border-radius:12px;padding:12px 14px;box-shadow:0 1px 3px rgba(15,23,42,.07)}
.card[data-id]{cursor:pointer}
.rot{font-size:.68rem;font-weight:700;letter-spacing:.08em;text-transform:uppercase;color:var(--muted)}
.val{font-size:1.75rem;font-weight:700;line-height:1.25;font-variant-numeric:tabular-nums}
.val.p{font-size:1.2rem;line-height:1.9}
.det{font-size:.78rem;color:var(--muted)}
.secao{font-size:.75rem;font-weight:700;letter-spacing:.09em;text-transform:uppercase;color:var(--accent);margin:18px 0 8px}
.graficos{display:grid;grid-template-columns:repeat(auto-fit,minmax(300px,1fr));gap:12px}
.graf{background:var(--surface);border:1px solid var(--border);border-radius:12px;padding:10px 12px;box-shadow:0 1px 3px rgba(15,23,42,.07)}
.graf h3{margin:2px 0 6px;font-size:.9rem}
.graf canvas{width:100%;height:230px;display:block;touch-action:pan-y}
details{background:var(--surface);border:1px solid var(--border);border-radius:12px;padding:10px 14px;margin-top:16px}
summary{cursor:pointer;font-weight:700;font-size:.9rem}
.cfg{display:grid;grid-template-columns:repeat(auto-fit,minmax(150px,1fr));gap:12px;margin-top:12px}
label{display:block;font-size:.72rem;font-weight:700;color:var(--muted);margin-bottom:4px}
select,input{width:100%;padding:8px 10px;border-radius:8px;border:1px solid var(--border);background:var(--surface);color:var(--text);font-size:.9rem}
button{background:var(--accent);color:var(--on);border:0;border-radius:8px;padding:9px 16px;font-weight:700;font-size:.85rem;cursor:pointer}
.rodape{text-align:center;color:var(--muted);font-size:.72rem;margin:22px 0 6px}
</style>
</head>
<body>
<div class="wrap">
<div class="banner">
<div><div class="eyebrow">MONITORAMENTO EM TEMPO REAL</div><div class="titulo" id="titulo">Temperatura e Umidade</div><div class="sub" id="sub">&nbsp;</div></div>
<div class="dir"><span id="badge" class="badge b-aviso">&#9679; AGUARDANDO</span><div class="relogio" id="relogio">--:--:--</div><div class="data" id="data"></div></div>
</div>
<div class="chips" id="chips"></div>
<div id="alerta" class="alerta info">Aguardando as primeiras leituras...</div>
<div id="vistaGeral">
<div class="secao" style="margin-top:4px">Sensores</div>
<div class="cards" id="cardsGeral"></div>
</div>
<div id="vistaSensor" style="display:none">
<div class="cards">
<div class="card"><div class="rot">Temperatura</div><div class="val" id="vT">--</div><div class="det" id="dT">&nbsp;</div></div>
<div class="card"><div class="rot">Umidade relativa</div><div class="val" id="vU">--</div><div class="det" id="dU">&nbsp;</div></div>
<div class="card" style="--c:var(--g3)"><div class="rot">Ponto de orvalho</div><div class="val" id="vO">--</div><div class="det">Temperatura de condens.</div></div>
<div class="card" style="--c:var(--muted)"><div class="rot">Sensação térmica</div><div class="val" id="vS">--</div><div class="det">Índice de calor (&ge; 27 &deg;C)</div></div>
</div>
<div class="cards">
<div class="card" style="--c:var(--ok)"><div class="rot">Estado do sensor</div><div class="val p" id="vE">--</div><div class="det" id="dE">&nbsp;</div></div>
<div class="card"><div class="rot">Leituras recebidas</div><div class="val p" id="vN">--</div><div class="det">desde que o painel foi ligado</div></div>
<div class="card"><div class="rot">Temperatura (período)</div><div class="val p" id="sT">--</div><div class="det">mín / média / máx</div></div>
<div class="card"><div class="rot">Umidade (período)</div><div class="val p" id="sU">--</div><div class="det">mín / média / máx</div></div>
</div>
</div>
<div id="secGraf">
<div class="secao">Evolução</div>
<div class="graficos">
<div class="graf"><h3>Temperatura (&deg;C)</h3><canvas id="gT"></canvas></div>
<div class="graf"><h3>Umidade (%)</h3><canvas id="gU"></canvas></div>
</div>
</div>
<details>
<summary>Configurações</summary>
<div class="cfg">
<div><label for="selTema">Tema</label><select id="selTema"><option value="claro">Claro</option><option value="escuro">Escuro</option></select></div>
<div><label for="selPaleta">Paleta de cores</label><select id="selPaleta"></select></div>
<div><label for="iTmin">Temperatura mínima (&deg;C)</label><input id="iTmin" type="number" step="0.5"></div>
<div><label for="iTmax">Temperatura máxima (&deg;C)</label><input id="iTmax" type="number" step="0.5"></div>
<div><label for="iUmin">Umidade mínima (%)</label><input id="iUmin" type="number" step="1"></div>
<div><label for="iUmax">Umidade máxima (%)</label><input id="iUmax" type="number" step="1"></div>
</div>
<p><button id="btnCsv" type="button">Exportar CSV</button></p>
</details>
<div class="rodape">Monitor Ambiental · os horários seguem o relógio do seu celular</div>
</div>
<script>
"use strict";
const $ = id => document.getElementById(id);
const esc = s => String(s).replace(/[&<>"']/g, c => ({"&":"&amp;","<":"&lt;",">":"&gt;",'"':"&quot;","'":"&#39;"}[c]));
const PALETAS = {
"Azul Corporativo":{a:"#1D4ED8",ad:"#60A5FA",s:"#0EA5E9",sd:"#38BDF8",g:["#0B2545","#13315C","#1D4ED8"]},
"Turquesa":{a:"#0E7490",ad:"#22D3EE",s:"#14B8A6",sd:"#5EEAD4",g:["#083344","#0E4F63","#0E7490"]},
"Esmeralda":{a:"#047857",ad:"#34D399",s:"#65A30D",sd:"#A3E635",g:["#052E26","#065F46","#059669"]},
"Índigo":{a:"#4338CA",ad:"#818CF8",s:"#9333EA",sd:"#C084FC",g:["#1E1B4B","#312E81","#4F46E5"]},
"Âmbar":{a:"#B45309",ad:"#FBBF24",s:"#EA580C",sd:"#FB923C",g:["#451A03","#78350F","#D97706"]},
"Grafite":{a:"#334155",ad:"#CBD5E1",s:"#94A3B8",sd:"#64748B",g:["#0F172A","#1E293B","#475569"]}
};
const MODOS = {
claro:{bg:"#F4F7FB",surface:"#FFFFFF",border:"#DCE5F0",text:"#0F172A",muted:"#5B6B82",grid:"#E5EAF2"},
escuro:{bg:"#0B1220",surface:"#111A2E",border:"#22304D",text:"#E6EDF7",muted:"#93A3BD",grid:"#1E2A44"}
};
const CORES = ["#2563EB","#10B981","#F59E0B","#EF4444","#8B5CF6","#EC4899","#14B8A6"];
const PADRAO = {tema:"claro",paleta:"Azul Corporativo",tmin:15,tmax:35,umin:30,umax:70};
const DIAS = ["domingo","segunda-feira","terça-feira","quarta-feira","quinta-feira","sexta-feira","sábado"];
const HIST_MAX = 400, JANELA_MS = 15 * 60 * 1000, ONLINE_S = 15;
let cfg = Object.assign({}, PADRAO);
try { cfg = Object.assign({}, PADRAO, JSON.parse(localStorage.getItem("cfgMonitor") || "{}")); } catch (e) {}
function salvar(){ try { localStorage.setItem("cfgMonitor", JSON.stringify(cfg)); } catch (e) {} }

let S = [], mapa = {}, dados = null, falhas = 0, intervaloMs = 2000, carregando = false, chipsSig = "";
let vista = "geral";
try { const q = new URLSearchParams(location.search).get("s"); if (q) vista = q; } catch (e) {}
const destaque = {gT:null, gU:null}, eixos = {};

const fmt = (v, c = 1) => (v !== null && v !== undefined && isFinite(v) ? v.toFixed(c).replace(".", ",") : "--");
const pad = n => String(n).padStart(2, "0");
const css = n => getComputedStyle(document.documentElement).getPropertyValue(n).trim();
const rotuloTempo = s => (s >= 60 ? Math.round(s / 60) + " min" : s + " s");
function duracao(s){
  s = Math.floor(s);
  const d = Math.floor(s / 86400), h = Math.floor(s % 86400 / 3600), m = Math.floor(s % 3600 / 60), x = s % 60;
  if (d) return d + "d " + h + "h " + m + "min";
  if (h) return h + "h " + m + "min";
  if (m) return m + "min " + x + "s";
  return x + "s";
}
function orvalho(t, u){
  u = Math.max(u, 0.1);
  const g = Math.log(u / 100) + 17.62 * t / (243.12 + t);
  return 243.12 * g / (17.62 - g);
}
function sensacao(t, u){
  const f = t * 9 / 5 + 32;
  if (f < 80) return t;
  let hi = -42.379 + 2.04901523*f + 10.14333127*u - 0.22475541*f*u - 0.00683783*f*f - 0.05481717*u*u + 0.00122874*f*f*u + 0.00085282*f*u*u - 0.00000199*f*f*u*u;
  if (u < 13 && f <= 112) hi -= ((13 - u) / 4) * Math.sqrt((17 - Math.abs(f - 95)) / 17);
  else if (u > 85 && f <= 87) hi += ((u - 85) / 10) * ((87 - f) / 5);
  return (hi - 32) * 5 / 9;
}
function resumo(pts, un){
  if (!pts.length) return "--";
  const a = pts.map(p => p[1]);
  return fmt(Math.min(...a)) + " / " + fmt(a.reduce((x, y) => x + y, 0) / a.length) + " / " + fmt(Math.max(...a)) + " " + un;
}
function aparar(a){
  const lim = Date.now() - JANELA_MS;
  while (a.length && (a[0][0] < lim || a.length > HIST_MAX)) a.shift();
}
function aplicarTema(){
  const m = MODOS[cfg.tema] || MODOS.claro, p = PALETAS[cfg.paleta] || PALETAS["Azul Corporativo"], esc2 = cfg.tema === "escuro";
  const r = document.documentElement.style;
  r.setProperty("--bg", m.bg); r.setProperty("--surface", m.surface); r.setProperty("--border", m.border);
  r.setProperty("--text", m.text); r.setProperty("--muted", m.muted); r.setProperty("--grid", m.grid);
  r.setProperty("--accent", esc2 ? p.ad : p.a); r.setProperty("--accent2", esc2 ? p.sd : p.s);
  r.setProperty("--g1", p.g[0]); r.setProperty("--g2", p.g[1]); r.setProperty("--g3", p.g[2]);
  r.setProperty("--on", esc2 ? "#0B1220" : "#FFFFFF");
  r.colorScheme = esc2 ? "dark" : "light";
}

function online(s){
  const x = s.cur;
  return !!x && x.idade_ms !== null && x.idade_ms / 1000 <= ONLINE_S && x.ok !== false;
}
function alertasDe(s){
  const x = s.cur, av = [];
  if (!x) return av;
  if (x.t === null){ av.push(s.id + ": sem leitura do sensor" + (s.local ? " local (verifique o DHT)" : "")); return av; }
  if (!online(s)){ av.push(s.id + ": sem receber leituras há " + duracao(x.idade_ms / 1000)); return av; }
  if (x.t > cfg.tmax) av.push(s.id + ": temperatura acima do limite máximo (" + fmt(cfg.tmax) + " °C) — " + fmt(x.t) + " °C");
  if (x.t < cfg.tmin) av.push(s.id + ": temperatura abaixo do limite mínimo (" + fmt(cfg.tmin) + " °C) — " + fmt(x.t) + " °C");
  if (x.u > cfg.umax) av.push(s.id + ": umidade acima do limite máximo (" + fmt(cfg.umax) + " %) — " + fmt(x.u) + " %");
  if (x.u < cfg.umin) av.push(s.id + ": umidade abaixo do limite mínimo (" + fmt(cfg.umin) + " %) — " + fmt(x.u) + " %");
  return av;
}

// Gráfico com eixo de tempo comum; aceita várias séries [{pts, cor, nome}]
function desenhar(id, series, lmin, lmax, un){
  const cv = $(id), w = cv.clientWidth, h = cv.clientHeight;
  if (!w || !h) return;
  const dpr = window.devicePixelRatio || 1;
  cv.width = w * dpr; cv.height = h * dpr;
  const c = cv.getContext("2d");
  c.setTransform(dpr, 0, 0, dpr, 0, 0); c.clearRect(0, 0, w, h);
  const grid = css("--grid"), mut = css("--muted"), dan = css("--danger"), sup = css("--surface"), bor = css("--border");
  c.font = "11px system-ui, sans-serif";
  const ativas = series.filter(s => s.pts.length);
  if (!ativas.length || ativas.every(s => s.pts.length < 2)){ c.fillStyle = mut; c.fillText("Aguardando dados...", 12, 24); return; }

  const agora = Date.now();
  let t0 = agora;
  ativas.forEach(s => { if (s.pts[0][0] < t0) t0 = s.pts[0][0]; });
  const span = Math.max(agora - t0, 30000);
  t0 = agora - span;
  eixos[id] = {t0: t0, span: span};

  let lo = lmin, hi = lmax;
  ativas.forEach(s => s.pts.forEach(p => { if (p[1] < lo) lo = p[1]; if (p[1] > hi) hi = p[1]; }));
  const fol = Math.max((hi - lo) * 0.12, 0.5); lo -= fol; hi += fol;

  const L = 42, R = 10, T = 22, B = 22, pw = w - L - R, ph = h - T - B;
  const X = ts => L + pw * (ts - t0) / span, Y = v => T + ph * (1 - (v - lo) / (hi - lo));

  c.lineWidth = 1; c.strokeStyle = grid; c.fillStyle = mut; c.textAlign = "right";
  for (let k = 0; k <= 4; k++){
    const v = lo + (hi - lo) * k / 4, y = Y(v);
    c.beginPath(); c.moveTo(L, y); c.lineTo(w - R, y); c.stroke();
    c.fillText(fmt(v), L - 6, y + 4);
  }
  c.textAlign = "center";
  [0, 0.5, 1].forEach(f => {
    const x = Math.min(Math.max(L + pw * f, L + 16), w - R - 16);
    c.fillText(f === 1 ? "agora" : "-" + rotuloTempo(Math.round(span / 1000 * (1 - f))), x, h - 6);
  });

  c.setLineDash([6, 4]); c.strokeStyle = dan; c.lineWidth = 1.2;
  [lmin, lmax].forEach(v => { if (v >= lo && v <= hi){ const y = Y(v); c.beginPath(); c.moveTo(L, y); c.lineTo(w - R, y); c.stroke(); } });
  c.setLineDash([]);

  ativas.forEach(s => {
    if (s.pts.length < 2) return;
    if (ativas.length === 1){
      const gr = c.createLinearGradient(0, T, 0, T + ph);
      gr.addColorStop(0, s.cor + "55"); gr.addColorStop(1, s.cor + "00");
      c.beginPath(); c.moveTo(X(s.pts[0][0]), T + ph);
      s.pts.forEach(p => c.lineTo(X(p[0]), Y(p[1])));
      c.lineTo(X(s.pts[s.pts.length - 1][0]), T + ph); c.closePath(); c.fillStyle = gr; c.fill();
    }
    c.beginPath();
    s.pts.forEach((p, i) => { if (i) c.lineTo(X(p[0]), Y(p[1])); else c.moveTo(X(p[0]), Y(p[1])); });
    c.strokeStyle = s.cor; c.lineWidth = 2; c.stroke();
  });

  const d = destaque[id];
  if (d !== null){
    c.strokeStyle = mut; c.lineWidth = 1; c.beginPath(); c.moveTo(X(d), T); c.lineTo(X(d), T + ph); c.stroke();
    const linhas = [];
    ativas.forEach(s => {
      let m = null;
      s.pts.forEach(p => { if (m === null || Math.abs(p[0] - d) < Math.abs(m[0] - d)) m = p; });
      if (m){
        c.fillStyle = s.cor; c.beginPath(); c.arc(X(m[0]), Y(m[1]), 4, 0, 6.2832); c.fill();
        linhas.push({cor: s.cor, txt: s.nome + ": " + fmt(m[1]) + " " + un});
      }
    });
    linhas.push({cor: mut, txt: "há " + rotuloTempo(Math.max(0, Math.round((agora - d) / 1000)))});
    let bw = 0;
    linhas.forEach(l => { bw = Math.max(bw, c.measureText(l.txt).width); });
    bw += 16;
    const bh = linhas.length * 14 + 8, bx = X(d) > L + pw / 2 ? L + 6 : L + pw - bw - 6;
    c.fillStyle = sup; c.strokeStyle = bor; c.lineWidth = 1; c.fillRect(bx, T + 4, bw, bh); c.strokeRect(bx, T + 4, bw, bh);
    c.textAlign = "left";
    linhas.forEach((l, i) => { c.fillStyle = l.cor; c.fillText(l.txt, bx + 8, T + 4 + 14 * (i + 1)); });
  }
}
function desenharTudo(){
  const lista = vista === "geral" ? S : (mapa[vista] ? [mapa[vista]] : []);
  desenhar("gT", lista.map(s => ({pts: s.pt, cor: s.cor, nome: s.id})), cfg.tmin, cfg.tmax, "°C");
  desenhar("gU", lista.map(s => ({pts: s.pu, cor: s.cor, nome: s.id})), cfg.umin, cfg.umax, "%");
}
["gT", "gU"].forEach(id => {
  const cv = $(id);
  const mover = ev => {
    const e = eixos[id];
    if (!e) return;
    const r = cv.getBoundingClientRect();
    const f = Math.max(0, Math.min(1, (ev.clientX - r.left - 42) / (r.width - 52)));
    destaque[id] = e.t0 + f * e.span;
    desenharTudo();
  };
  cv.addEventListener("pointermove", mover);
  cv.addEventListener("pointerdown", mover);
  cv.addEventListener("pointerleave", () => { destaque[id] = null; desenharTudo(); });
});

function cartaoResumo(s){
  const x = s.cur, ok = !!x && x.t !== null, on = online(s);
  const st = !ok ? "Sem leitura" : (on ? "Online" : "Sem dados");
  const idade = ok ? " · há " + duracao(x.idade_ms / 1000) : "";
  return '<div class="card" style="--c:' + s.cor + '" data-id="' + esc(s.id) + '"><div class="rot">' + esc(s.id) + (s.local ? " · local" : "") +
    '</div><div class="val">' + (ok ? fmt(x.t) + " °C" : "--") + '</div><div class="det">' + (ok ? fmt(x.u) + " %" : "--") + " · " + st + idade + "</div></div>";
}
function preencherSensor(s){
  const x = s.cur, ok = !!x && x.t !== null, on = online(s);
  $("vT").textContent = ok ? fmt(x.t) + " °C" : "--"; $("vU").textContent = ok ? fmt(x.u) + " %" : "--";
  $("dT").textContent = "Limites: " + fmt(cfg.tmin) + " a " + fmt(cfg.tmax); $("dU").textContent = "Limites: " + fmt(cfg.umin) + " a " + fmt(cfg.umax);
  $("vO").textContent = ok ? fmt(orvalho(x.t, x.u)) + " °C" : "--"; $("vS").textContent = ok ? fmt(sensacao(x.t, x.u)) + " °C" : "--";
  $("vE").textContent = ok ? (on ? "Online" : "Sem dados") : "Sem leitura";
  $("dE").textContent = (ok ? "Última leitura há " + duracao(x.idade_ms / 1000) : "") + (s.local && x && x.erros ? (ok ? " · " : "") + x.erros + " falhas" : "");
  $("vN").textContent = x ? x.n : "--";
  $("sT").textContent = resumo(s.pt, "°C"); $("sU").textContent = resumo(s.pu, "%");
}
function construirChips(){
  const sig = vista + "|" + S.map(s => s.id).join(",");
  if (sig === chipsSig) return;
  chipsSig = sig;
  let h = '<button type="button" class="chip' + (vista === "geral" ? " on" : "") + '" data-v="geral">Visão geral</button>';
  S.forEach(s => {
    h += '<button type="button" class="chip' + (vista === s.id ? " on" : "") + '" data-v="' + esc(s.id) + '"><span class="pt" style="background:' + s.cor + '"></span>' + esc(s.id) + "</button>";
  });
  $("chips").innerHTML = h;
}
function selecionar(v){
  vista = v;
  try { history.replaceState(null, "", v === "geral" ? "/" : "/?s=" + encodeURIComponent(v)); } catch (e) {}
  renderizar(); desenharTudo();
}

function renderizar(){
  const geral = vista === "geral", sel = geral ? null : (mapa[vista] || null);
  construirChips();
  $("vistaGeral").style.display = geral ? "" : "none";
  $("vistaSensor").style.display = sel ? "" : "none";
  $("secGraf").style.display = (geral || sel) ? "" : "none";

  if (geral){ $("titulo").textContent = "Temperatura e Umidade"; $("sub").textContent = S.length + (S.length === 1 ? " sensor" : " sensores"); }
  else { $("titulo").textContent = vista; $("sub").textContent = sel ? (sel.local ? "Sensor local do painel" : "Sensor remoto") : "Aguardando o primeiro envio deste sensor"; }
  if (geral) $("cardsGeral").innerHTML = S.map(cartaoResumo).join("");
  if (sel) preencherSensor(sel);

  const b = $("badge"), a = $("alerta");
  let cls = "b-aviso", txt = "● AGUARDANDO";
  if (falhas > 2){ cls = "b-erro"; txt = "● SEM COMUNICAÇÃO"; }
  else if (sel){ const on = online(sel); cls = on ? "b-ok" : "b-aviso"; txt = on ? "● SENSOR ONLINE" : "● SEM DADOS"; }
  else if (geral && S.length){ const n = S.filter(online).length; cls = n === S.length ? "b-ok" : (n ? "b-aviso" : "b-erro"); txt = "● " + n + "/" + S.length + " ONLINE"; }
  b.className = "badge " + cls; b.textContent = txt;

  let av = [];
  if (geral) S.forEach(s => { av = av.concat(alertasDe(s)); }); else if (sel) av = alertasDe(sel);
  if (falhas > 2){ a.className = "alerta erro"; a.textContent = "Sem comunicação com o ESP32 do painel. Verifique a conexão Wi-Fi."; }
  else if (!dados){ a.className = "alerta info"; a.textContent = "Aguardando as primeiras leituras..."; }
  else if (!geral && !sel){ a.className = "alerta info"; a.textContent = "Aguardando o primeiro envio do sensor " + vista + "..."; }
  else if (av.length){ a.className = "alerta erro"; a.innerHTML = "<strong>Alertas ativos</strong><br>" + av.map(esc).join("<br>"); }
  else { a.className = "alerta ok"; a.textContent = "Todos os parâmetros estão dentro dos limites configurados."; }
}

async function buscar(url){
  const r = await fetch(url, {cache: "no-store"});
  if (!r.ok) throw new Error("HTTP " + r.status);
  return r.json();
}
async function carregarHistorico(){
  if (carregando) return;
  carregando = true;
  try {
    for (let i = 0; i < S.length; i++){
      const s = S[i];
      try {
        const h = await buscar("/api/historico?id=" + encodeURIComponent(s.id));
        const agora = Date.now();
        s.pt = h.t.map((v, k) => [agora - h.idade_ms[k], v]);
        s.pu = h.u.map((v, k) => [agora - h.idade_ms[k], v]);
        aparar(s.pt); aparar(s.pu);
      } catch (e) {}
    }
  } finally { carregando = false; }
  desenharTudo(); renderizar();
}
async function atualizar(){
  try {
    const d = await buscar("/api/dados");
    falhas = 0; dados = d; intervaloMs = d.intervalo_ms || 2000;
    const agora = Date.now();
    let novo = false;
    d.sensores.forEach((x, i) => {
      let s = mapa[x.id];
      if (!s){ s = mapa[x.id] = {id: x.id, pt: [], pu: [], ultN: -1, cor: CORES[i % CORES.length]}; S.push(s); novo = true; }
      s.local = x.local; s.cur = x;
      if (x.t !== null && x.n !== s.ultN){
        const ts = agora - x.idade_ms, ult = s.pt.length ? s.pt[s.pt.length - 1][0] : 0;
        if (ts - ult > 1000){ s.pt.push([ts, x.t]); s.pu.push([ts, x.u]); }
        s.ultN = x.n;
      }
      aparar(s.pt); aparar(s.pu);
    });
    if (novo) carregarHistorico();
  } catch (e) { falhas++; }
  renderizar(); desenharTudo();
}
async function ciclo(){ await atualizar(); setTimeout(ciclo, intervaloMs); }

function relogio(){
  const d = new Date();
  $("relogio").textContent = pad(d.getHours()) + ":" + pad(d.getMinutes()) + ":" + pad(d.getSeconds());
  $("data").textContent = DIAS[d.getDay()] + ", " + pad(d.getDate()) + "/" + pad(d.getMonth() + 1) + "/" + d.getFullYear();
}

function exportarCsv(){
  const lista = vista === "geral" ? S : (mapa[vista] ? [mapa[vista]] : []);
  const linhas = ["Horario;Sensor;Temperatura (C);Umidade (%)"];
  lista.forEach(s => s.pt.forEach((p, i) => {
    linhas.push(new Date(p[0]).toLocaleString("pt-BR") + ";" + s.id + ";" + p[1].toFixed(1).replace(".", ",") + ";" + (s.pu[i] ? s.pu[i][1].toFixed(1).replace(".", ",") : ""));
  }));
  const blob = new Blob(["\ufeff" + linhas.join("\n")], {type: "text/csv;charset=utf-8"});
  const a = document.createElement("a");
  a.href = URL.createObjectURL(blob); a.download = "leituras.csv"; a.click();
}

function iniciarConfig(){
  const sp = $("selPaleta");
  Object.keys(PALETAS).forEach(n => { const o = document.createElement("option"); o.value = n; o.textContent = n; sp.appendChild(o); });
  $("selTema").value = cfg.tema; sp.value = cfg.paleta;
  $("iTmin").value = cfg.tmin; $("iTmax").value = cfg.tmax; $("iUmin").value = cfg.umin; $("iUmax").value = cfg.umax;
  $("selTema").onchange = e => { cfg.tema = e.target.value; salvar(); aplicarTema(); desenharTudo(); };
  sp.onchange = e => { cfg.paleta = e.target.value; salvar(); aplicarTema(); desenharTudo(); };
  [["iTmin", "tmin"], ["iTmax", "tmax"], ["iUmin", "umin"], ["iUmax", "umax"]].forEach(([id, k]) => {
    $(id).onchange = e => {
      const v = parseFloat(e.target.value);
      if (isFinite(v)){ cfg[k] = v; salvar(); renderizar(); desenharTudo(); }
    };
  });
  $("btnCsv").onclick = exportarCsv;
  $("chips").addEventListener("click", e => { const b = e.target.closest("[data-v]"); if (b) selecionar(b.getAttribute("data-v")); });
  $("cardsGeral").addEventListener("click", e => { const b = e.target.closest("[data-id]"); if (b) selecionar(b.getAttribute("data-id")); });
}

aplicarTema(); iniciarConfig(); relogio(); setInterval(relogio, 1000);
window.addEventListener("resize", desenharTudo);
renderizar();
ciclo();
setInterval(carregarHistorico, 60000);
</script>
</body>
</html>
)rawliteral";