#version 120

varying vec3 coord;
varying vec3 position;

uniform mat4 rotation_matrix;
uniform vec3 color;
uniform float time;

uniform samplerCube skybox_texture;
uniform sampler2D noise_texture;

const mat3 m = mat3( 0.00,  0.80,  0.60,
                    -0.80,  0.36, -0.48,
                    -0.60, -0.48,  0.64 );

float hash31(vec3 p) {
    return fract(sin(dot(p, vec3(127.1, 311.7, 321.4))) * 43758.5453123);
}

//TODO: replace with noise texture
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

void main()
{
    vec3 surf2view = normalize(position); surf2view = vec3(rotation_matrix * vec4(surf2view, 0.0));
    
    vec3 q = surf2view * 8.0;
    
    float luma  = 0.5000 * noise(q); q = m * q * 2.01;
          luma += 0.3500 * noise(q); q = m * q * 2.02;
          luma += 0.1250 * noise(q); q = m * q * 2.03;
          luma += 0.0635 * noise(q); q = m * q * 2.01;
    
    
    gl_FragColor = vec4(vec3(luma * (color)), 1.0);
}
