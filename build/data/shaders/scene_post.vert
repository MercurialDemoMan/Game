#version 120

varying vec3 position;
varying vec3 normal;
varying vec2 coord;

void main() {
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    position    = vec3(gl_ModelViewMatrix * gl_Vertex);
    normal      = gl_NormalMatrix * gl_Normal;
    coord       = gl_MultiTexCoord0.xy;
}
