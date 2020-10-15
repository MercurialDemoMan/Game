#version 120

varying vec3 coord;
varying vec3 position;

uniform samplerCube skybox_texture;
uniform sampler2D   noise_texture;
uniform mat4        rotation_matrix;
uniform vec4        color;
uniform float       time;

vec2 hash22(vec2 p) {
    return fract(vec2(2097152.0, 32768.0) * sin(dot(p, vec2(7.0, 157.0))));
}

void main() {
    vec3 surf2view = normalize(position); surf2view = vec3(rotation_matrix * vec4(surf2view, 0.0));
    /*float t = time / 13.0 + cos(time / 2.0);
    
    vec4 color = vec4(
                          0.5 + 0.5 * (sin(surf2view.x * cos(t) * 6.0 * cos(surf2view.y * t / 232.0)) + sin(surf2view.y * sin(t) * 3.0)),
                          0.5 + 0.5 * (cos(surf2view.x * sin(t) * 5.0 * cos(surf2view.y * t / 211.0)) + sin(surf2view.y * sin(t) * 6.0)),
                          0.5 + 0.5 * (cos(surf2view.x * cos(t) * 4.0 * cos(surf2view.y * t / 311.0)) + sin(surf2view.y * sin(t) * 7.0)),
                          1.0);*/
    
    float f = sin(surf2view.x + sin(2.0 * surf2view.y + time)) + sin(length(surf2view) + time) + 0.5 * sin(surf2view.x * 2.5 + time);
    
    // color
    vec3 col = 0.7 + 0.3 * cos(f + vec3(0.0, 2.1, 4.2));
    
    gl_FragColor = vec4(col, 1.0);
}
