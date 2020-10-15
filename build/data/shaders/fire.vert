#version 120

attribute vec3 in_pos;
attribute vec3 in_nor;

varying vec3 position;
varying vec3 normal;
varying vec2 coord;
varying vec3 l_p;

uniform vec3 light_pos;
uniform vec3 obj_pos;
uniform float g_time;

float rng(vec3 s) {
    vec3 num = abs(sin(s)) * 10000.0f;
    return fract(length(num));
}

void main() {
    
    vec3 modified_pos = normalize(in_pos) * sin(rng(obj_pos + in_pos) * 100.0f + g_time * 10.0) / 15.0;
    
    gl_Position = gl_ModelViewProjectionMatrix * (vec4(in_pos, 1.0) + vec4(modified_pos, 0.0));
    position    = vec3(gl_ModelViewMatrix * (vec4(in_pos, 1.0) + vec4(modified_pos, 0.0)));
    l_p         = vec3(gl_ModelViewMatrix * vec4(light_pos, 0.0));
    
    normal  = gl_NormalMatrix * in_nor;
    coord   = in_pos.xz;
}
