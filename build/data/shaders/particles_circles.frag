#version 120

varying vec2 coord;

uniform vec3 color;
uniform float blur;

void main() {
    float radius      = length(vec2(0.5) - coord);
    /*float d = (1.0 - step(0.3, length(r))) - (1.0 - step(0.25, length(r)));
    gl_FragColor = vec4(color * d, d);*/
    float blur_factor = (blur * 6.0 - 3.0) / 10.0;
    float alpha_ch    = smoothstep(0.4, blur_factor, radius);
    gl_FragColor      = vec4(color * alpha_ch, 1.0);
}
