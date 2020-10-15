#version 120

varying vec2 coord;

uniform sampler2D sampler;
uniform sampler2D blurred_sampler;

void main() {
    vec4 b_c     = texture2D(sampler, coord);
    float val    = (b_c.r + b_c.g + b_c.b) / 3.0;
    gl_FragColor = texture2D(blurred_sampler, coord) * pow((1.0 - val), 2.0) + texture2D(sampler, coord);
}
