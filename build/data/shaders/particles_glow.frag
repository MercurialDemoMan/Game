#version 120

varying vec2 coord;

uniform sampler2D sampler;
uniform vec3 color;

void main() {
    vec3  glow  = vec3(0.0);
    float alpha = 0.0;
    vec4  tex   = texture2D(sampler, coord);
    
    if(tex.a == 0.0) {
        tex.rgb = vec3(0.0);
        vec2  d = vec2(0.5, 0.5) - coord;
        float l = length(d);
        glow    = color;
        alpha   = max(0.0, pow(0.01, l) - 0.1) / 2.0;
    }
    gl_FragColor = tex + vec4(glow, alpha);
}
