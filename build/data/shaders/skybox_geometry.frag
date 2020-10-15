#version 120

varying vec3 coord;
varying vec3 position;

uniform samplerCube skybox_texture;
uniform sampler2D   noise_texture;
uniform mat4        rotation_matrix;
uniform vec3        color;
uniform float       time;

#define DRUGS 100
#define M_PI 3.14

vec2 hash22(vec2 p) {
    return fract(vec2(2097152.0, 32768.0) * sin(dot(p, vec2(7.0, 157.0))));
}

vec3 rotx(vec3 p, float a) {
    float s = sin(a);
    float c = cos(a);
    return vec3(p.x, c*p.y - s*p.z, s*p.y + c*p.z);
}
vec3 roty(vec3 p, float a){
    float s = sin(a);
    float c = cos(a);
    return vec3(c*p.x + s*p.z, p.y, -s*p.x + c*p.z);
}
vec3 rotz(vec3 p, float a){
    float s = sin(a);
    float c = cos(a);
    return vec3(c*p.x - s*p.y, s*p.x + c*p.y, p.z);
}

vec3 tex(in vec2 p)
{
    float frq = 10.3;
    p += 0.405;
    return vec3(1.0) * smoothstep(0.9, 1.05, max(sin((p.x)*frq),sin((p.y)*frq)));
}

vec3 cubeproj(in vec3 p)
{
    //vec3 x = tex(p.zy / p.x);
    vec3 y = tex(p.xz / p.y);
    //vec3 z = tex(p.xy / p.z);
    
    //simple coloring/shading
    //x *= vec3(1,0,0) * abs(p.x) + p.x * vec3(0,1,0);
    y *= color * p.y;// + p.y * vec3(0,0,1);
    //z *= vec3(0,0,1) * abs(p.z) + p.z * vec3(1,0,0);
    
    //select face
    //p = abs(p);
    //if (p.x > p.y && p.x > p.z) return x;
    //if (p.y > p.x && p.y > p.z) return y;
    return y * 2.0;
}

void main() {
    vec3 surf2view = normalize(position);
         surf2view = vec3(-rotation_matrix * vec4(surf2view, 0.0));
    
    gl_FragColor = vec4(cubeproj(surf2view), 1.0);// + texture2D(noise_texture, hash22(surf2view.xz)) / 32.0;
}
