#version 120
varying vec2 coord;

uniform sampler2D sampler;


void main() {
    vec4 color   = texture2D(sampler, coord);
    float luma   = (color.r * 0.2126) + (color.g * 0.7152) + (color.b * 0.0722);
    gl_FragColor = color * luma * 2.0;
}
