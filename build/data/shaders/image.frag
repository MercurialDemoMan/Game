#version 120

varying vec2 coord;

uniform sampler2D image;

uniform vec3  color;

void main() 
{
    gl_FragColor = texture2D(image, coord) * vec4(color, 1.0);
}
