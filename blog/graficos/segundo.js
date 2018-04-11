@info
type your project information here
and the author

@libs
http://webglplayground.net/libs/jquery-1.6.2.min.js
http://webglplayground.net/libs/Three_r46.js

@main
// The JQuery library facilitates DOM operations and normalizes parameters for input events across the browsers

var renderer = new THREE.WebGLRenderer();
renderer.setSize(viewportWidth, viewportHeight);
$('#' + containerId).append(renderer.domElement);

var scene = new THREE.Scene();
var camera = new THREE.OrthographicCamera(-1, 1, 1, -1, 0.1, 100.0);
camera.position.z = 1.0;
scene.add( camera );

var uniforms = {
  resolution : { type:"v2", value:new THREE.Vector2(viewportWidth,viewportHeight) },
  time: { type:"f", value: 1 },
  mouse: { type:"v2", value:new THREE.Vector2(2,4) },
 // mouseLeft: { type:"i", value:0 },
  tex0: { type:"t", value:0, texture: THREE.ImageUtils.loadTexture("/samples/concrete1.jpg") }
};
uniforms.tex0.texture.wrapS = uniforms.tex0.texture.wrapT = THREE.Repeat;
var uniforms2 = {
  resolution : { type:"v2", value:new THREE.Vector2(viewportWidth,viewportHeight) },
  time: { type:"f", value: 1 },
  mouse: { type:"v2", value:new THREE.Vector2(0,0) },
  //mouseLeft: { type:"i", value:0 },
  tex0: { type:"t", value:0, texture: THREE.ImageUtils.loadTexture("/samples/concrete1.jpg") }
};
uniforms2.tex0.texture.wrapS = uniforms2.tex0.texture.wrapT = THREE.Repeat;


var material = new THREE.ShaderMaterial({
  uniforms: uniforms,
  vertexShader: glsl_vs1,
  fragmentShader: glsl_fs1
});
var material2 = new THREE.ShaderMaterial({
  uniforms: uniforms,
  vertexShader: glsl_vs2,
  fragmentShader: glsl_fs2
});
var geometry = new THREE.Geometry();
geometry.vertices.push(new THREE.Vertex(new THREE.Vector3(-1, -0.8, 0)));
geometry.vertices.push(new THREE.Vertex(new THREE.Vector3(1, -0.8, 0)));
geometry.vertices.push(new THREE.Vertex(new THREE.Vector3(1, 0.8, 0)));
geometry.vertices.push(new THREE.Vertex(new THREE.Vector3(-1, 0.8, 0)));
geometry.faces.push(new THREE.Face4(0,1,2,3));

var s = viewportWidth/512.0;
var t = viewportHeight/512.0;

// Three.js does not support flipping y coordinate of a loaded textures
// so we do it manually
geometry.faceVertexUvs[0].push([
  new THREE.UV(0,t),
  new THREE.UV(s,t),
  new THREE.UV(s,0),
  new THREE.UV(0,0)
]);


var geometry2 = new THREE.Geometry();
geometry2.vertices.push(new THREE.Vertex(new THREE.Vector3(0.4, 0.5, 0)));



geometry2.vertices.push(new THREE.Vertex(new THREE.Vector3(-0.5, 0.5, 0)));
geometry2.vertices.push(new THREE.Vertex(new THREE.Vector3(-0.5, -0.5, 0)));
geometry2.vertices.push(new THREE.Vertex(new THREE.Vector3(0.4, -0.5, 0)));





//geometry2.vertices.push(new THREE.Vertex(new THREE.Vector3(-0.5, 0.5, 0)));
geometry2.faces.push(new THREE.Face3(0,1,2),new THREE.Face3(1,2,3));




// Three.js does not support flipping y coordinate of a loaded textures
// so we do it manually
geometry2.faceVertexUvs[0].push([
  new THREE.UV(0,t),
  new THREE.UV(s,t),
  new THREE.UV(s,0),
  new THREE.UV(0,0)
]);
var mesh = new THREE.Mesh(geometry, material);
var mesh2 = new THREE.Mesh(geometry2, material2);

scene.addObject(mesh2);
scene.addObject(mesh);
/*
 * for a complete list of JQuery keyboard and mouse events see:
 * keyboard events: http://api.jquery.com/category/events/keyboard-events/
 * mouse events: http://api.jquery.com/category/events/mouse-events/
 *
 * alternatively you can use document.addEventListener for 'mousedown', 'touchstart', 'keydown' etc.
 * e.g., document.addEventListener( 'mousedown', onDocumentMouseDown, false );
 */
$(document).keydown( function(event) {
  event.preventDefault();
  var key = event.which;
});

$(document).mousemove( function(event) {
  event.preventDefault();
  // transforming cursor coordinates to [-1.0, 1.0] range
  // [0,0] being in the left bottom corner to match the vertex coordinates
  var x = (event.pageX / viewportWidth)*2.0 - 1.0;
  var y = 0.0 - ((event.pageY / viewportHeight)*2.0 - 1.0);
  uniforms.mouse.value.set(x, y);
});

$(document).mousedown( function(event) {
  event.preventDefault();
  var key = event.which;
  var x = event.pageX;
  var y = event.pageY;
  if (key==1) {
    uniforms.mouseLeft.value = 1;
  }
});

$(document).mouseup( function(event) {
  event.preventDefault();
  var key = event.which;
  if (key==1) {
    uniforms.mouseLeft.value = 0;
  }
});

$(document).mouseleave( function(event) {
  event.preventDefault();
  uniforms.mouse.value.set(0,0);
});

// invoked every frame
function frame(runningTime, deltaTime) {
  uniforms.time.value = runningTime;	
  renderer.render(scene, camera);
}

@glsl_vs1
varying vec2 t;
varying vec2 pos;

void main() {
  vec4 mvPosition = modelViewMatrix * vec4( position, 1.0 );
  gl_Position = projectionMatrix * mvPosition;
  t = uv;
  pos = vec2(position.x, position.y);
}

@glsl_fs1
#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

void main() {
    vec2 p = gl_FragCoord.xy / resolution.x * 0.7;
    vec3 col;
    float t = time * mouse.x;
    for(float j = 0.0; j < 2.0; j++){
        for(float i = 1.0; i < 1.0; i++){
            p.x += 0.1 / (i + j) * sin(i * 10.0 * p.y /*+ t +*/ + cos((time / (12. * i)) /*/ i*/ + j));
            p.y += 0.1 / (i + j) * cos(i * 10.0 * p.x /*+ t +*/ + sin((time / (12. * i)) /*/ i*/ + j));
        }
        col[int(j)] = abs(p.x + p.y);
    }
    gl_FragColor = vec4(col, 1.);
}


@glsl_vs2
varying vec2 t;
varying vec2 pos;

void main() {
  vec4 mvPosition = modelViewMatrix * vec4( position, 1.0 );
  gl_Position = projectionMatrix * mvPosition;
  t = uv;
  pos = vec2(position.x, position.y);
}

@glsl_fs2
#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

void main() {
    vec2 p = gl_FragCoord.xy / resolution.x * 0.7;
    vec3 col;
    float t = time * mouse.x;
    for(float j = 0.0; j < 3.0; j++){
        for(float i = 1.0; i < 10.0; i++){
            p.x += 0.1 / (i + j) * sin(i * 10.0 * p.y + t + cos((time / (12. * i)) * i + j));
            p.y += 0.1 / (i + j)* cos(i * 10.0 * p.x + t + sin((time / (12. * i)) * i + j));
        }
        col[int(j)] = abs(p.x + p.y);
    }
    gl_FragColor = vec4(col, 1.);
}
