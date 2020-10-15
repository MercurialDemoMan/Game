#version 120

varying vec3 coord;
varying vec3 position;

uniform samplerCube skybox_texture;
uniform sampler2D noise_texture;
uniform mat4 rotation_matrix;
uniform vec3 color;
uniform float time;

vec3 sky_color(vec3 ray)
{
    //return exp2(-ray.y / vec3(.1,.3,.6)) / 6.0; // blue
    //return exp2(-ray.y / vec3(.18,.2,.28))*vec3(1,.95,.8) / 6.0; // overcast
    //return exp2(-ray.y / vec3(.1,.2,.8))*vec3(1,.75,.5) / 6.0; // dusk
    //return exp2(-ray.y / vec3(.03,.2,.9)) / 6.0; // tropical blue
    //return exp2(-ray.y / vec3(.4,.06,.01)) / 6.0; // orange-red
    //return exp2(-ray.y / vec3(0.1, 0.2,  0.01)) / 6.0; // green
    //return exp2(-ray.y / color.rgb * vec3(0.3, 0.8, 1.0)) / 6.0;
	return exp2(-ray.y / vec3(0.1, 0.1, 0.1)) / 24.0;
}

void main()
{
    vec3 surf2view = vec3(rotation_matrix * vec4(normalize(position), 0.0));
    
    vec3 tint = vec3(1.0);
    
    if (surf2view.y < 0.0) {
        surf2view.y = -surf2view.y;
        tint        = mix(vec3(0.1), tint, pow(1.0 - surf2view.y, 10.0));
    }
    
    gl_FragColor = vec4(pow(sky_color(surf2view * tint), vec3(1.0 / 2.2)), 1.0);
}
