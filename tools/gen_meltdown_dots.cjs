#!/usr/bin/env node
// Sample the approved meltdown layout into A8 masks.
// macOS font files are references only and are not copied into firmware.
const fs = require('fs');
const path = require('path');
const deps = '/Users/winola/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/node_modules';
const {createCanvas, GlobalFonts} = require(path.join(deps, '@napi-rs/canvas'));
const sharp = require(path.join(deps, 'sharp'));

GlobalFonts.registerFromPath(path.join(__dirname, '../managed_components/lvgl__lvgl/scripts/built_in_font/SourceHanSansSC-Normal.otf'), 'Han');
GlobalFonts.registerFromPath('/System/Library/Fonts/Avenir Next Condensed.ttc', 'Display');
GlobalFonts.registerFromPath('/System/Library/Fonts/Menlo.ttc', 'Digits');

const outDir = path.join(__dirname, '..', 'assets', 'dots');
const themeArg = process.argv.indexOf('--theme');
const themeName = themeArg >= 0 ? process.argv[themeArg + 1] : 'light';
if (!['dark', 'light'].includes(themeName)) throw Error('Use --theme dark|light');
const dark = {bg:'#34393A',ink:'#F5F6F1',dim:'#BAC3C2',blue:'#43C5FA',hi:'#B8EDFF',up:'#FF9862',down:'#59CADA',zero:'#A3B4AE',future:'#707B7C',heat:['#A3B4AE','#59CADA','#F2C563','#FF9862','#FF6865']};
const light = {bg:'#C5C8C2',ink:'#283437',dim:'#586765',blue:'#006DAD',hi:'#238BB5',up:'#D5671B',down:'#2674B8',zero:'#B1B6AF',future:'#ACB5AE',heat:['#B1B6AF','#E6C7BA','#D99D86','#BD6750','#862F2D']};
// Legacy sample literals map onto the selected palette, including shared heat roles.
const reference = {ink:'#F5F6F1',dim:'#BAC3C2',blue:'#66B4D8',hi:'#C0E9F1',up:'#D28A69',down:'#8BB3BD',zero:'#7C8789',future:'#707B7C',heat:['#626A6B','#8BB3BD','#BD9F77','#D28A69','#DD766F']};
const theme = themeName === 'light' ? light : dark;
function themedColor(color) {
 for(const key of ['ink','dim','blue','hi','up','down','zero','future']) if(color.toUpperCase()===reference[key].toUpperCase())return theme[key];
 const level=reference.heat.indexOf(color.toUpperCase());
 return level>=0?theme.heat[level]:color;
}



// Native-resolution asset authoring. Glyphs stay continuous; the paper field has no holes.
const images = [], phrases = [], glyphs = [[], [], []];
function addImage(px,w,h) { const id=images.length; images.push({id,px,w,h}); return id; }
function addPhrase(key,r) { const id=addImage(r.px,r.w,r.h); const p={key,id,x:r.x||0,y:r.y||0,w:r.w,h:r.h}; phrases.push(p); return p; }
function trim(px,w,h) {
 let l=w,r=-1,t=h,b=-1;
 for(let y=0;y<h;y++) for(let x=0;x<w;x++) if(px[y*w+x]>12){l=Math.min(l,x);r=Math.max(r,x);t=Math.min(t,y);b=Math.max(b,y);}
 if(r<l) return {w:1,h:1,px:Buffer.alloc(1)};
 const nw=r-l+1,nh=b-t+1,out=Buffer.alloc(nw*nh);
 for(let y=0;y<nh;y++) px.copy(out,y*nw,(t+y)*w+l,(t+y)*w+r+1);
 return {w:nw,h:nh,px:out};
}
function maskText(text,height,font='Han',weight=400) {
 const c=createCanvas(4000,240),g=c.getContext('2d');
 g.font=`${weight} 160px "${font}"`;g.fillStyle='white';g.fillText(text,10,180);
 const d=g.getImageData(0,0,c.width,c.height).data;
 let l=c.width,r=0,t=c.height,b=0;
 for(let y=0;y<c.height;y++)for(let x=0;x<c.width;x++)if(d[(y*c.width+x)*4+3]>10){l=Math.min(l,x);r=Math.max(r,x);t=Math.min(t,y);b=Math.max(b,y);}
 const w=Math.max(1,Math.round((r-l+1)*height/(b-t+1))),h=height;
 const out=createCanvas(w*4,h*4),ctx=out.getContext('2d');
 ctx.drawImage(c,l,t,r-l+1,b-t+1,0,0,w*4,h*4);
 const raw=ctx.getImageData(0,0,w*4,h*4).data,px=Buffer.alloc(w*h);
 for(let y=0;y<h;y++)for(let x=0;x<w;x++){
  let sum=0;for(let j=0;j<4;j++)for(let i=0;i<4;i++)sum+=raw[((y*4+j)*w*4+x*4+i)*4+3];
  px[y*w+x]=Math.round(sum/16);
 }
 return {w,h,px};
}
function texture(r) { return r; }
function blockOf(text,opt) {return texture(maskText(text,opt.h,opt.font||'Han',opt.weight||400));}
function placeBlock(key,r,x,y,anchor,valign){
 if(anchor==='center')x-=r.w/2;if(anchor==='right')x-=r.w;
 if(valign==='center')y-=r.h/2;if(valign==='bottom')y-=r.h;
 return addPhrase(key,{...r,x:Math.round(x),y:Math.round(y)});
}
function phrase(key,h,x,y,anchor,valign){return placeBlock(key,blockOf(key,{h}),x,y,anchor,valign);}
function shape(kind,w,h){
 const c=createCanvas(w*4,h*4),g=c.getContext('2d');g.scale(4,4);
 g.fillStyle='white';g.strokeStyle='white';g.lineWidth=1;g.lineJoin='round';g.lineCap='round';
 if(kind==='triangle') {g.beginPath();g.moveTo(w/2,.5);g.lineTo(w-.5,h-.5);g.lineTo(.5,h-.5);g.closePath();g.fill();}
 else if(kind==='speaker') {
  g.beginPath();g.moveTo(1,h*.36);g.lineTo(4,h*.36);g.lineTo(7,h*.16);g.lineTo(7,h*.84);g.lineTo(4,h*.64);g.lineTo(1,h*.64);g.closePath();g.fill();
  for(const rad of [4,6.5]){g.beginPath();g.arc(6,h/2,rad,-.7,.7);g.stroke();}
 }else if(kind==='slash'){g.beginPath();g.moveTo(2,h-2);g.lineTo(w-2,2);g.stroke();}
 else {g.beginPath();g.roundRect(.75,.75,w-1.5,h-1.5,1.5);g.stroke();
  if(kind==='confirm'){
   const letters=['01110','10001','10001','10001','10001','10001','01110'];
   const k=['10001','10010','10100','11000','10100','10010','10001'];
   const ox=Math.floor((w-11)/2),oy=Math.floor((h-7)/2);
   for(let y=0;y<7;y++)for(let x=0;x<5;x++){
    if(letters[y][x]==='1')g.fillRect(ox+x,oy+y,1,1);
    if(k[y][x]==='1')g.fillRect(ox+6+x,oy+y,1,1);
   }
  }
  else{g.beginPath();g.moveTo(w*.3,kind==='up'?h*.6:h*.4);g.lineTo(w*.5,kind==='up'?h*.4:h*.6);g.lineTo(w*.7,kind==='up'?h*.6:h*.4);g.stroke();}
 }
 const raw=g.getImageData(0,0,w*4,h*4).data,px=Buffer.alloc(w*h);
 for(let y=0;y<h;y++)for(let x=0;x<w;x++){let sum=0;for(let j=0;j<4;j++)for(let i=0;i<4;i++)sum+=raw[((y*4+j)*w*4+x*4+i)*4+3];px[y*w+x]=Math.round(sum/16);}
 return texture({w,h,px},kind==='triangle');
}
function icon(key,kind,x,cy,w=12,h=12){return addPhrase(key,{...shape(kind,w,h),x,y:cy-h/2});}
const MENLO_H=11,menloAdvance=8;
glyphs[0][32]={id:-1,advance:menloAdvance,w:0,h:0};
const monoCanvas=createCanvas(240,240),mg=monoCanvas.getContext('2d');
mg.font='400 160px "Digits"';mg.fillStyle='white';mg.fillText('0',20,180);
const md=mg.getImageData(0,0,240,240).data;let mt=240,mb=0;
for(let y=0;y<240;y++)for(let x=0;x<240;x++)if(md[(y*240+x)*4+3]>10){mt=Math.min(mt,y);mb=Math.max(mb,y);}
const monoScale=MENLO_H/(mb-mt+1);
for(const ch of '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ.-/%'){
 mg.clearRect(0,0,240,240);mg.fillText(ch,20,180);
 const small=createCanvas(menloAdvance*4,MENLO_H*4),sg=small.getContext('2d');
 sg.drawImage(monoCanvas,20,mt,menloAdvance/monoScale,mb-mt+1,0,0,menloAdvance*4,MENLO_H*4);
 const raw=sg.getImageData(0,0,menloAdvance*4,MENLO_H*4).data,px=Buffer.alloc(menloAdvance*MENLO_H);
 for(let y=0;y<MENLO_H;y++)for(let x=0;x<menloAdvance;x++){
  let sum=0;for(let j=0;j<4;j++)for(let i=0;i<4;i++)sum+=raw[((y*4+j)*menloAdvance*4+x*4+i)*4+3];px[y*menloAdvance+x]=Math.round(sum/16);
 }
 const r=texture({w:menloAdvance,h:MENLO_H,px}),id=addImage(r.px,r.w,r.h);
 glyphs[0][ch.charCodeAt(0)]={id,advance:menloAdvance,w:menloAdvance,h:MENLO_H};
}
for(const face of [1,2])for(const ch of (face===1?'0123456789+':'0123456789+-')){
 const h=face===1?108:(ch==='+'||ch==='-'?19:28);
 const r=texture(maskText(ch,h,'Display',face===1?700:600),true);
 const id=addImage(r.px,r.w,r.h);glyphs[face][ch.charCodeAt(0)]={id,advance:r.w+(face===1?5:2),w:r.w,h:r.h};
}
glyphs[2][32]={id:-1,advance:5,w:0,h:0};
// Header: all visible bounds have the same center Y, separate from the date.
icon('icon:speaker','speaker',213,22,14,12);
icon('icon:slash','slash',213,22,14,12);
icon('icon:mute-key','confirm',27,256,24,16);
phrase('长按切换',13,61,256,null,'center');
phrase('静音',13,121,256,null,'center');
phrase('数据统计',15,120,59,'center');
phrase('牛马的崩溃瞬间，只有自己知道',12,120,282,'center','center');
for(const key of ['今日崩溃','日期未确认','日期回拨','未归档'])phrase(key,15,120,59,'center');
phrase('次',13,206,197,null,'bottom');
phrase('本周',14,36,104);
phrase('较昨日',14,36,210);
icon('icon:ok','confirm',27,226,24,16);
phrase('崩溃时按一下',14,61,226,null,'center');
phrase('记入今日',12,42,302,null,'center');
phrase('保持分开',12,176,302,null,'center');
function centeredNav(label){
 const text=blockOf(label,{h:12});
 const x=Math.floor((240-12-6-text.w)/2);
 return {x,text,labelX:x+18};
}
const statsNav=centeredNav('统计');
icon('icon:stats','up',statsNav.x,22,12,12);
placeBlock('统计',statsNav.text,statsNav.labelX,22,null,'center');
const monthNav=centeredNav('本月');
icon('icon:down','down',monthNav.x,302,12,12);
placeBlock('本月',monthNav.text,monthNav.labelX,302,null,'center');
const homeNav=centeredNav('返回今日');
icon('icon:up','up',homeNav.x,22,12,12);
icon('icon:stats-back','down',homeNav.x,302,12,12);
placeBlock('返回今日',homeNav.text,homeNav.labelX,22,null,'center');
for(const day of ['周一','周二','周三','周四','周五','周六','周日'])phrase(day,13,8,48);
for(const key of ['蓝牙配网','正在校时','校时失败','请打开小程序','记入今日, 保持分开'])phrase(key,13,120,176,'center');
phrase('写入失败',12,128,205);
placeBlock('未归档 ',blockOf('未归档',{h:12}),16,205);
const tri=addPhrase('icon:tri-up',{...shape('triangle',16,17),x:36,y:253});
const inv=Buffer.alloc(16*17),tp=images[tri.id].px;
for(let y=0;y<17;y++)tp.copy(inv,(16-y)*16,y*16,(y+1)*16);
addPhrase('icon:tri-down',{x:36,y:253,w:16,h:17,px:inv});
// Visible geometry checks run before generating any firmware assets.
const bounds=key=>phrases.find(p=>p.key===key);
const middle=p=>p.y+p.h/2;
if(middle(bounds('icon:speaker'))!==22 || middle(bounds('统计'))!==22)throw Error('top center');
if(middle(bounds('返回今日'))!==22 || middle(bounds('icon:up'))!==22)throw Error('return nav');
if(middle(bounds('本月'))!==302 || middle(bounds('icon:down'))!==302)throw Error('month nav');
if(bounds('icon:stats-back').x!==bounds('icon:up').x || middle(bounds('icon:stats-back'))!==302)throw Error('stats nav');
if(Math.abs(middle(bounds('icon:mute-key'))-middle(bounds('长按切换')))>.5)throw Error('mute center');
if(Math.abs(middle(bounds('icon:ok'))-middle(bounds('崩溃时按一下')))>.5)throw Error('record center');
if(bounds('icon:tri-up').y+bounds('icon:tri-up').h!==270)throw Error('triangle baseline');
if(bounds('较昨日').x!==bounds('icon:tri-up').x)throw Error('comparison left edge');
function rowSpan(y){
 const radius=30,width=240,height=320;
 if(y<0||y>=height)return null;
 if(y>=radius&&y<height-radius)return {x1:0,x2:width-1};
 const edge=y<radius?radius-y:y-(height-1-radius);let inset=0;
 while((inset+1)*(inset+1)+edge*edge<=radius*radius)inset++;
 return {x1:radius-inset,x2:width-radius+inset-1};
}
for(const p of phrases){if(p.key.startsWith('周'))continue;
 for(let y=p.y;y<p.y+p.h;y++){const span=rowSpan(y);if(!span||p.x<span.x1||p.x+p.w-1>span.x2)throw Error('bezel clips '+p.key);}
}
function cellDots(side,ring){
 const c=createCanvas(side*4,side*4),g=c.getContext('2d');g.scale(4,4);
 g.fillStyle='white';g.strokeStyle='white';g.lineWidth=1;
 g.beginPath();g.roundRect(.5,.5,side-1,side-1,2);
 if(ring)g.stroke();else g.fill();
 const raw=g.getImageData(0,0,side*4,side*4).data,px=Buffer.alloc(side*side);
 for(let y=0;y<side;y++)for(let x=0;x<side;x++){
  let sum=0;for(let j=0;j<4;j++)for(let i=0;i<4;i++)sum+=raw[((y*4+j)*side*4+x*4+i)*4+3];
  px[y*side+x]=Math.round(sum/16);
 }
 return {x:0,y:0,w:side,h:side,px};
}
const fillLg=cellDots(26,false),fillSm=cellDots(22,false),ringLg=cellDots(26,true),ringSm=cellDots(22,true);
const fillLgId=addImage(fillLg.px,26,26),fillSmId=addImage(fillSm.px,22,22),ringLgId=addImage(ringLg.px,26,26),ringSmId=addImage(ringSm.px,22,22);
function bytes(px) {
    const lines = [];
    for (let i = 0; i < px.length; i += 16) {
        const chunk = [];
        for (let j = i; j < Math.min(i + 16, px.length); j++) chunk.push('0x' + px[j].toString(16).padStart(2, '0'));
        lines.push('    ' + chunk.join(', ') + ',');
    }
    return lines.join('\n');
}

function symbolFor(id) {
    if (id === fillLgId) return {name: 'melt_fill_lg', storage: ''};
    if (id === fillSmId) return {name: 'melt_fill_sm', storage: ''};
    if (id === ringLgId) return {name: 'melt_ring_lg', storage: ''};
    if (id === ringSmId) return {name: 'melt_ring_sm', storage: ''};
    return {name: 'melt_img_' + id, storage: 'static '};
}

function imageC(id) {
    const image = images[id];
    const symbol = symbolFor(id);
    return `${symbol.storage}const uint8_t ${symbol.name}_px[] __attribute__((aligned(4))) = {\n${bytes(image.px)}\n};\n${symbol.storage}const lv_image_dsc_t ${symbol.name} = {\n    .header.magic = LV_IMAGE_HEADER_MAGIC,\n    .header.cf = LV_COLOR_FORMAT_A8,\n    .header.flags = 0,\n    .header.w = ${image.w},\n    .header.h = ${image.h},\n    .header.stride = ${image.w},\n    .data_size = sizeof ${symbol.name}_px,\n    .data = ${symbol.name}_px,\n};\n`;
}

const mesh=[];
for(let y=0;y<24;y++)for(let x=0;x<24;x++){
 const grain=((x*17+y*31+x*y*7)%5)-2;
 const base=themeName==='light'?[197,200,194]:[52,57,58];
 const c=base.map((channel)=>Math.max(0,Math.min(255,channel+grain)));
 mesh.push(((c[0]>>3)<<11)|((c[1]>>2)<<5)|(c[2]>>3));
}
const meshC=`const uint16_t melt_mesh_px[576] = {${mesh.join(',')}};
const lv_image_dsc_t melt_mesh = {
.header.magic=LV_IMAGE_HEADER_MAGIC, .header.cf=LV_COLOR_FORMAT_RGB565,
.header.w=24,.header.h=24,.header.stride=48,.data_size=sizeof melt_mesh_px,.data=(const uint8_t *)melt_mesh_px
};\n`;
let body = `/* Generated by tools/gen_meltdown_dots.cjs. Font files are not packed. */\n#include "meltdown_dots.h"\n\n#include <string.h>\n\n`;
body += meshC;
images.forEach((image, index) => {
    body += imageC(index) + '\n';
});
body += 'static const melt_phrase_t melt_phrases[] = {\n';
for (const phrase of phrases) {
    const text = phrase.key.replace(/\\/g, '\\\\').replace(/"/g, '\\"');
    body += `    {"${text}", &${symbolFor(phrase.id).name}, ${phrase.x}, ${phrase.y}},\n`;
}
body += `};\n\nstatic const melt_glyph_t melt_glyphs[3][128] = {\n`;
for (let face = 0; face < 3; face++) {
    body += `    [${face}] = {\n`;
    for (let code = 0; code < 128; code++) {
        const glyph = glyphs[face][code];
        if (!glyph) continue;
        const src = glyph.id < 0 ? 'NULL' : `&${symbolFor(glyph.id).name}`;
        body += `        [${code}] = {${src}, ${glyph.advance}},\n`;
    }
    body += '    },\n';
}
body += `};\n
const melt_phrase_t *melt_dots_phrase(const char *text)
{
    if (!text) return NULL;
    for (size_t i = 0; i < sizeof melt_phrases / sizeof melt_phrases[0]; i++) {
        if (strcmp(melt_phrases[i].text, text) == 0) return &melt_phrases[i];
    }
    return NULL;
}

const melt_glyph_t *melt_dots_glyph(int face, char ascii)
{
    const unsigned char code = (unsigned char)ascii;
    if (face < 0 || face > 2 || code > 127) return NULL;
    const melt_glyph_t *glyph = &melt_glyphs[face][code];
    if (!glyph->image && glyph->advance == 0) return NULL;
    return glyph;
}
`;

const paletteDefines = Object.entries(theme).filter(([key])=>key!=='heat').map(([key,value])=>`#define MELT_COLOR_${key.toUpperCase()} 0x${value.slice(1)}`).join('\n')+'\n'+theme.heat.map((value,i)=>`#define MELT_HEAT_${i} 0x${value.slice(1)}`).join('\n');
const header = `#pragma once

/* Generated palette: ${themeName}. Regenerate with --theme dark|light. */
${paletteDefines}

#include "lvgl.h"

#include <stdint.h>

#define MELT_FACE_MENLO 0
#define MELT_FACE_HERO 1
#define MELT_FACE_DELTA 2

typedef struct {
    const char *text;
    const lv_image_dsc_t *image;
    int16_t x;
    int16_t y;
} melt_phrase_t;

typedef struct {
    const lv_image_dsc_t *image;
    uint8_t advance;
} melt_glyph_t;

const melt_phrase_t *melt_dots_phrase(const char *text);
const melt_glyph_t *melt_dots_glyph(int face, char ascii);

extern const lv_image_dsc_t melt_mesh;
extern const lv_image_dsc_t melt_fill_lg;
extern const lv_image_dsc_t melt_fill_sm;
extern const lv_image_dsc_t melt_ring_lg;
extern const lv_image_dsc_t melt_ring_sm;
`;

fs.mkdirSync(outDir, {recursive: true});
fs.writeFileSync(path.join(outDir, 'meltdown_dots.h'), header);
fs.writeFileSync(path.join(outDir, 'meltdown_dots.c'), body);

function hex(value) {
    return [parseInt(value.slice(1, 3), 16), parseInt(value.slice(3, 5), 16), parseInt(value.slice(5, 7), 16)];
}
function stamp(frame, raster, color, dx, dy) {
    const [cr, cg, cb] = hex(themedColor(color));
    for (let y = 0; y < raster.h; y++) {
        for (let x = 0; x < raster.w; x++) {
            const a = raster.px[y * raster.w + x] / 255;
            if (a <= 0) continue;
            const X = dx + raster.x + x;
            const Y = dy + raster.y + y;
            if (X < 0 || Y < 0 || X >= 240 || Y >= 320) continue;
            const i = (Y * 240 + X) * 3;
            frame[i] = Math.round(frame[i] * (1 - a) + cr * a);
            frame[i + 1] = Math.round(frame[i + 1] * (1 - a) + cg * a);
            frame[i + 2] = Math.round(frame[i + 2] * (1 - a) + cb * a);
        }
    }
}
function paintPhrase(frame, key, color) {
    const phrase = phrases.find((item) => item.key === key);
    stamp(frame, {x: 0, y: 0, w: phrase.w, h: phrase.h, px: images[phrase.id].px}, color || '#F5F6F1', phrase.x, phrase.y);
}
function paintPhraseAt(frame, key, x, y, color) {
    const phrase = phrases.find((item) => item.key === key);
    stamp(frame, {x: 0, y: 0, w: phrase.w, h: phrase.h, px: images[phrase.id].px}, color || '#F5F6F1', x, y);
}
function paintRun(frame, text, face, left, top, color) {
    let x = left;
    for (const ch of text) {
        const glyph = glyphs[face][ch.charCodeAt(0)];
        if (!glyph) continue;
        if (glyph.id >= 0) {
            const image = images[glyph.id];
            stamp(frame, {x: 0, y: 0, w: image.w, h: image.h, px: image.px}, color || '#F5F6F1', x, top);
        }
        x += glyph.advance;
    }
}
function boxMask(text,face,w,h,bottom) {
 let natural=0,maxH=0;
 for(const ch of text){const g=glyphs[face][ch.charCodeAt(0)];if(!g)continue;natural+=g.advance;if(g.id>=0)maxH=Math.max(maxH,images[g.id].h);}
 let scale=256;if(natural>w)scale=Math.floor(w*256/natural);if(maxH>h)scale=Math.min(scale,Math.floor(h*256/maxH));scale=Math.max(1,scale);
 const sh=Math.floor(maxH*scale/256);let x=bottom?0:Math.floor((w-Math.floor(natural*scale/256))/2);
 const y0=bottom?h-sh:Math.floor((h-sh)/2),px=Buffer.alloc(w*h);
 for(const ch of text){const g=glyphs[face][ch.charCodeAt(0)];if(!g)continue;
  if(g.id>=0){const im=images[g.id],dw=Math.max(1,Math.floor(im.w*scale/256)),dh=Math.max(1,Math.floor(im.h*scale/256)),gy=y0+sh-dh;
   for(let yy=0;yy<dh;yy++)for(let xx=0;xx<dw;xx++){
    if(x+xx<0||x+xx>=w||gy+yy<0||gy+yy>=h)continue;
    px[(gy+yy)*w+x+xx]=Math.max(px[(gy+yy)*w+x+xx],im.px[Math.floor(yy*im.h/dh)*im.w+Math.floor(xx*im.w/dw)]);
   }
  }x+=Math.floor(g.advance*scale/256);
 }
 if(!bottom){let l=w,r=-1;for(let y=0;y<h;y++)for(let x=0;x<w;x++)if(px[y*w+x]){l=Math.min(l,x);r=Math.max(r,x);}
  if(r>=l){const shift=Math.floor((w-r+l-1)/2)-l,copy=Buffer.from(px);px.fill(0);
   for(let y=0;y<h;y++)for(let x=l;x<=r;x++)px[y*w+x+shift]=copy[y*w+x];
  }
 }
 return {x:0,y:0,w,h,px};
}
function paintBox(frame,text,face,x,y,w,h,color,bottom){stamp(frame,boxMask(text,face,w,h,bottom),color,x,y);}
for(const n of ['00','01','08','11','100','999','10000','99999+']){
 const m=boxMask(n,1,160,108,false);let l=160,r=-1;
 for(let y=0;y<108;y++)for(let x=0;x<160;x++)if(m.px[y*160+x]){l=Math.min(l,x);r=Math.max(r,x);}
 if(Math.abs((l+r+1)/2-80)>.5)throw Error('uncentered '+n);
}
function blank() {
    const frame = Buffer.alloc(240 * 320 * 3);
    for(let y=0;y<320;y++)for(let x=0;x<240;x++){
      const c=mesh[(y%24)*24+x%24],i=(y*240+x)*3;
      frame[i]=Math.round(((c>>11)&31)*255/31);frame[i+1]=Math.round(((c>>5)&63)*255/63);frame[i+2]=Math.round((c&31)*255/31);
    }
    return frame;
}
function applyBezel(frame) {
    for(let i=0;i<frame.length;i+=3){
        frame[i]=Math.round((frame[i]>>3)*255/31);
        frame[i+1]=Math.round((frame[i+1]>>2)*255/63);
        frame[i+2]=Math.round((frame[i+2]>>3)*255/31);
    }
    for (let y = 0; y < 320; y++) {
        const span = rowSpan(y);
        for (let x = 0; x < 240; x++) {
            if (span && x >= span.x1 && x <= span.x2) continue;
            const i = (y * 240 + x) * 3;
            frame[i] = 0;
            frame[i + 1] = 0;
            frame[i + 2] = 0;
        }
    }
}
function heatColor(count){return theme.heat[count===0?0:count<=2?1:count<=5?2:count<=9?3:4];}
function paintBars(frame) {
 const heights=[10,17,0,20,27,0,0],bottom=275;
 for(let i=0;i<7;i++){
  if(!heights[i])continue;
  stamp(frame,{x:0,y:0,w:7,h:heights[i],px:Buffer.alloc(7*heights[i],255)},'#66B4D8',16+i*12,bottom-heights[i]);
  stamp(frame,{x:0,y:0,w:7,h:1,px:Buffer.alloc(7,255)},'#C0E9F1',16+i*12,bottom-heights[i]);
 }
}
function paintMonth(frame) {
    const cell = 26;
    const gap = 4;
    const columns = 5;
    const width = columns * cell + (columns - 1) * gap;
    const height = 7 * cell + 6 * gap;
    const originX = 64 + Math.floor((146 - width) / 2);
    const originY = 75 + Math.floor((206 - height) / 2);
    const headers = [1, 5, 12, 19, 26];
    const days = ['周一', '周二', '周三', '周四', '周五', '周六', '周日'];
    const menloH = glyphs[0]['0'.charCodeAt(0)].h;
    for (let column = 0; column < columns; column++) {
        const header = String(headers[column]).padStart(2, '0');
        const center = originX + column * (cell + gap) + Math.floor(cell / 2);
        const left = center - menloAdvance;
        paintRun(frame, header, 0, left, originY - 15 - Math.floor(menloH / 2), '#F5F6F1');
    }
    for (let row = 0; row < 7; row++) {
        const phrase = phrases.find((item) => item.key === days[row]);
        const centerY = originY + row * (cell + gap) + Math.floor(cell / 2);
        stamp(frame, {x: 0, y: 0, w: phrase.w, h: phrase.h, px: images[phrase.id].px}, '#F5F6F1',
            originX - 8 - phrase.w, centerY - Math.floor(phrase.h / 2));
        for (let column = 0; column < columns; column++) {
            const index = column * 7 + row;
            const day = index - 3 + 1;
            const x = originX + column * (cell + gap);
            const y = originY + row * (cell + gap);
            if (day < 1 || day > 31) continue;
            if (day > 23) {
                stamp(frame, {x: 0, y: 0, w: 26, h: 26, px: images[ringLgId].px}, '#707B7C', x, y);
                continue;
            }
            const counts=[0,2,4,0,6,3,1,10,2,0,4,6,2,1,0,12,3,4,3,5,0,6,8];
            const color=heatColor(counts[day-1]);
            stamp(frame, {x: 0, y: 0, w: 26, h: 26, px: images[fillLgId].px}, color, x, y);
            if (day === 23) stamp(frame, {x: 0, y: 0, w: 26, h: 26, px: images[ringLgId].px}, '#F5F6F1', x, y);
        }
    }
}

const menloTop = 22 - Math.floor(glyphs[0]['0'.charCodeAt(0)].h / 2);
const today = blank();
paintRun(today, '10.23 FRI', 0, 14, menloTop, '#F5F6F1');
paintRun(today, '86%', 0, 213 - 4 - 3 * menloAdvance, menloTop, '#F5F6F1');
paintPhrase(today, 'icon:speaker');
paintPhrase(today, 'icon:mute-key');
paintPhrase(today, '长按切换');
paintPhrase(today, '今日崩溃');
paintBox(today, '08', 1, 40, 89, 160, 108, heatColor(8), false);
paintPhrase(today, '次');
paintPhrase(today, '静音');
paintPhrase(today, '统计');
paintPhrase(today, 'icon:stats');
paintPhrase(today, '牛马的崩溃瞬间，只有自己知道');
paintPhrase(today, 'icon:ok');
paintPhrase(today, '崩溃时按一下');
paintPhrase(today, 'icon:down');
paintPhrase(today, '本月');
applyBezel(today);

const month = blank();
paintRun(month, '2026.10', 0, 14, menloTop, '#F5F6F1');
paintMonth(month);
paintPhrase(month, 'icon:up');
paintPhrase(month, '返回今日');
applyBezel(month);

const stats=blank();
paintRun(stats,'10.23 FRI',0,14,menloTop,'#F5F6F1');
for(const key of ['icon:speaker','数据统计','本周','较昨日','icon:stats-back'])paintPhrase(stats,key);
const backLabel=bounds('返回今日');
paintPhraseAt(stats,'返回今日',backLabel.x,Math.round(302-backLabel.h/2));
paintPhrase(stats,'icon:tri-up','#D28A69');
paintBox(stats,'2',2,62,242,100,28,'#F5F6F1',true);
const barFrame=Buffer.alloc(240*320*3);paintBars(barFrame);
for(let y=248;y<275;y++)for(let x=16;x<100;x++){
 const i=(y*240+x)*3;if(!barFrame[i]&&!barFrame[i+1]&&!barFrame[i+2])continue;
 for(let dy=0;dy<2;dy++)for(let dx=0;dx<2;dx++){
  const j=((128+(y-248)*2+dy)*240+36+(x-16)*2+dx)*3;
  barFrame.copy(stats,j,i,i+3);
 }
}
applyBezel(stats);
(async () => {
    await sharp(stats,{raw:{width:240,height:320,channels:3}}).png().toFile('/tmp/meltdown-preview-stats.png');
    await sharp(stats,{raw:{width:240,height:320,channels:3}}).resize(480,640,{kernel:'nearest'}).png().toFile('/tmp/meltdown-preview-stats-2x.png');
    const todayPath = '/tmp/meltdown-preview-today.png';
    const monthPath = '/tmp/meltdown-preview-month.png';
    await sharp(today, {raw: {width: 240, height: 320, channels: 3}}).png().toFile(todayPath);
    await sharp(month, {raw: {width: 240, height: 320, channels: 3}}).png().toFile(monthPath);
    await sharp(today, {raw: {width: 240, height: 320, channels: 3}})
        .resize(480, 640, {kernel: 'nearest'}).png().toFile('/tmp/meltdown-preview-today-2x.png');
    await sharp(month, {raw: {width: 240, height: 320, channels: 3}})
        .resize(480, 640, {kernel: 'nearest'}).png().toFile('/tmp/meltdown-preview-month-2x.png');
    const bytesTotal = images.reduce((sum, image) => sum + image.px.length, 0);
    const box = (key) => {
        const phrase = phrases.find((item) => item.key === key);
        return {x: phrase.x, y: phrase.y, w: phrase.w, h: phrase.h, r: phrase.x + phrase.w, b: phrase.y + phrase.h};
    };
    console.log(JSON.stringify({
        theme: themeName,
        images: images.length,
        phrases: phrases.length,
        bytes: bytesTotal,
        menloAdvance,
        menloH: glyphs[0]['0'.charCodeAt(0)].h,
        hero0: glyphs[1]['0'.charCodeAt(0)],
        deltaPlus: glyphs[2]['+'.charCodeAt(0)],
        mute: box('长按切换'),
        title: box('今日崩溃'),
        unit: box('次'),
        footer: box('崩溃时按一下'),
        monthLabel: box('本月'),
        back: box('返回今日'),
        motto: box('牛马的崩溃瞬间，只有自己知道'),
        speaker: box('icon:speaker'),
    }, null, 2));
})();
