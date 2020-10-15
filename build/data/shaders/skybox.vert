#version 120

attribute vec3 in_pos;
attribute vec3 in_nor;
attribute vec2 in_uv;

varying vec3 position;

/**
 * \brief entry point
 */
void main() {
    gl_Position = gl_ModelViewProjectionMatrix * vec4(in_pos, 1.0);
    position    = vec3(gl_ModelViewMatrix * vec4(in_pos, 1.0));
}
