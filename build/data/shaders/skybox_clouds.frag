#version 120

varying vec3 coord;
varying vec3 position;

uniform sampler2D noise_texture;
uniform mat4      rotation_matrix;
uniform float     time;

vec2 hash22(vec2 p) {
    return fract(vec2(2097152.0, 32768.0) * sin(dot(p, vec2(7.0, 157.0))));
}

float hash31(vec3 p) {
    return fract(sin(dot(p, vec3(127.1, 311.7, 321.4))) * 43758.5453123);
}

float noise(vec3 p)
{
    p.z += time * 0.75;
    
    vec3 i = floor(p);
    vec3 f = fract(p);
    f     *= f * (3.0 - 2.0 * f);
    
    vec2 c = vec2(0, 1);
    
    return mix(
               mix(mix(hash31(i + c.xxx), hash31(i + c.yxx), f.x),
                   mix(hash31(i + c.xyx), hash31(i + c.yyx), f.x),
                   f.y),
               mix(mix(hash31(i + c.xxy), hash31(i + c.yxy), f.x),
                   mix(hash31(i + c.xyy), hash31(i + c.yyy), f.x),
                   f.y),
               f.z);
}

float fbm(vec3 p) {
    float f = 0.0;
    f += 0.50000 * noise(1.0 * p);
    f += 0.25000 * noise(2.0 * p);
    f += 0.12500 * noise(4.0 * p);
    f += 0.06250 * noise(8.0 * p);
    return f;
}

vec3 cubemap(vec3 d, vec3 c1, vec3 c2) {
    return fbm(d) * mix(c1, c2, d * .5 + .5);
}

void main() {
    vec3 surf2view = normalize(position); surf2view = vec3(rotation_matrix * vec4(surf2view, 0.0));
    
    gl_FragColor = vec4(cubemap(surf2view, vec3(.3,.9,.6), vec3(.5,.25,.9)), 1.0) + texture2D(noise_texture, hash22(surf2view.xz * time)) / 16.0;
}
