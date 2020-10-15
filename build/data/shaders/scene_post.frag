#version 120

#define EDGE 2.0

varying vec3 position;
varying vec3 normal;
varying vec2 coord;

uniform sampler2D sampler;
uniform sampler2D particles_sampler;

uniform float g_time;
uniform float shade_time;
uniform float speed;
uniform vec2 dims;

uniform float motion_blur_samples[10];
uniform float pixel_factor;

uniform int bayer[64];/* = {
    0, 128,  38, 160,  16, 136,  45, 168,
    191,  66, 224,  96, 199,  74, 233, 104,
    51, 176,  22, 142,  58, 184,  30, 152,
    241, 111, 207,  81, 249, 119, 216,  88,
    20, 139,  47, 171,  10, 132,  41, 163,
    204,  77, 237, 107, 196,  70, 228, 100,
    63, 188,  33, 155,  54, 179,  26, 146,
    254, 123, 220,  92, 245, 116, 212,  85
};*/

int m_mod(int x, int y) {
    return x - y * int(x/y);
}

float hash(vec2 p) {
    return fract(dot(p + vec2(0.36834, 0.723), normalize(fract(p.yx * 73.91374) + 1e-4)) * 7.38734);
}

float rand(vec3 p) {
    p  = fract(p * 0.4183099) * 17.0;
    return fract(p.x * p.y * p.z * (p.x + p.y + p.z));
}

void main() {
    //pixelation
    vec2 pix      = ceil(coord * dims / pixel_factor) * pixel_factor;
    vec2 uv_curve = pix / dims;
    
    //cubic distortion
    float r2 = (uv_curve.x - 0.5) * (uv_curve.x - 0.5) + (uv_curve.y - 0.5) * (uv_curve.y - 0.5);
    float f  = 1.0 + r2 * ((-speed * 2.0) * sqrt(r2));
    uv_curve = f * (uv_curve - 0.5) + 0.5;
    
    //get the color
    vec4 FragColor = texture2D(sampler, uv_curve);
    
    //motion blur
    vec2 dir   = 0.5 - uv_curve;
    float dist = sqrt(dir.x * dir.x + dir.y * dir.y);
    dir        = dir / dist;
    
    vec4 color = texture2D(sampler, uv_curve) + vec4(texture2D(particles_sampler, uv_curve).rgb, 0.0);
    vec4 sum = color;
    
    for (int i = 0; i < 10; i++) {
        sum += texture2D( sampler, uv_curve + dir * motion_blur_samples[i] * 0.5)
            + vec4(texture2D(particles_sampler, uv_curve + dir * motion_blur_samples[i] * 0.5).rgb, 0.0);
    }
    
    sum      *= 1.0 / 11.0;
    float t   = clamp(dist * speed * 5.0, 0.0, 1.0);
    
    FragColor = mix(FragColor, sum, t) + texture2D(particles_sampler, uv_curve);
    
    
    //vingette
    float d_l      = length(vec2(0.5, 0.5) - coord);
    FragColor.rgb -= max(0.0, exp(d_l * d_l * d_l) - 1);
    
    //color roundification (retro)
    FragColor.r = ceil(FragColor.r * 255.0 / 16.0) * 16.0 / 256.0;
    FragColor.g = ceil(FragColor.g * 255.0 / 16.0) * 16.0 / 256.0;
    FragColor.b = ceil(FragColor.b * 255.0 / 16.0) * 16.0 / 256.0;
    //dithering
    int tmp_x = int(coord.x * dims.x);
    int tmp_y = int(coord.y * dims.y);
    int tmp_d_x = int(dims.x * 10.0);
    int tmp_d_y = int(dims.y * 10.0);
    int ind_x = m_mod(m_mod(tmp_x, tmp_d_x), 8);
    int ind_y = m_mod(m_mod(tmp_y, tmp_d_x), 8);
    float dither = float(bayer[ind_y * 8 + ind_x]) / 255.0;
    
    //final color
    gl_FragColor = vec4(FragColor.rgb * shade_time, 1.0); /*step(dither, vec4(FragColor.rgb * shade_time, 1.0));*/
}
