#version 120

varying vec2 coord;
uniform mat4 rot_mat;

void main() {
    gl_Position = gl_ModelViewProjectionMatrix * rot_mat * gl_Vertex;
    coord       = gl_MultiTexCoord0.xy;
}
