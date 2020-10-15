#version 120

varying vec3 coord;
varying vec3 position;

uniform samplerCube skybox_texture;
uniform sampler2D   noise_texture;
uniform mat4        rotation_matrix;
uniform vec3        color;
uniform float       time;

/**
 * \brief entry point
 */
void main()
{
	//get fragment to camera vector
    vec3 frag_to_cam     = vec3(rotation_matrix * vec4(normalize(position), 0.0));
    vec3 frag_to_cam_alt = frag_to_cam;
    
	//create vector modifiers
    vec2 displacement = vec2(sin(time + frag_to_cam.y * 10.0) / 10.0,
                             cos(time + frag_to_cam.y * 10.0) / 10.0);
    
	//apply modifiers
    frag_to_cam.xz     += displacement;
    frag_to_cam_alt.xz -= displacement;
    
	//the more we look down, the darker the skybox will become
    float deep_dark = -(frag_to_cam.y - 0.75);
	
	//mix two skybox textures with modifiers + apply some noise
	gl_FragColor = ((textureCube(skybox_texture, frag_to_cam) * vec4(color, 1.0) + 
	                 textureCube(skybox_texture, frag_to_cam_alt) * vec4(1.0 - color.rgb, 1.0))) * vec4(deep_dark, deep_dark, deep_dark, 1.0);
}

