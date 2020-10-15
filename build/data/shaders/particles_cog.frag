#version 120

#define M_PI 3.14

varying vec2 coord;

uniform vec3 color;

void main() {
    vec2 pos = coord - vec2(0.5);
    
    float r  = length(pos);
    
    float a  = atan(pos.x, pos.y);
    
    float phase        = fract(0.5 / M_PI * 10.0 * a);
    float tooth_height = 4.0 * abs(phase - 0.5) - 1.0;
    
    float radius = 0.3 + 0.03 * clamp(tooth_height / 0.5, -1.0, 1.0);
    
    float d      = (1.0 - step(radius, r));
    
    gl_FragColor = vec4(vec3(1.0), d);
}
