#version 120

varying vec2 coord;

uniform vec2 dims;

void main() {
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    coord       = gl_MultiTexCoord0.xy;
}

