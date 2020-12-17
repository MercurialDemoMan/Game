#version 120

attribute vec3 in_pos;
attribute vec3 in_nor;

varying vec3 position;
varying vec3 normal;
varying vec3 coord;
varying vec3 l_p;

uniform vec3 light_pos;
uniform vec3 predef_nor;

void main() {
    gl_Position = gl_ModelViewProjectionMatrix * vec4(in_pos, 1.0);
    position    = vec3(gl_ModelViewMatrix * vec4(in_pos, 1.0));
    l_p         = vec3(gl_ModelViewMatrix * vec4(light_pos, 0.0));
    
	if(predef_nor != vec3(0.0, 0.0, 0.0)
	{
		normal = pre_nor;
	}
	else
	{
		normal = gl_NormalMatrix * in_nor;
	}
    coord  = in_pos;
}
