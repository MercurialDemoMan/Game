#version 120

varying vec3 coord;
varying vec3 position;

uniform samplerCube skybox_texture;
uniform sampler2D   noise_texture;
uniform mat4        rotation_matrix;
uniform vec3        color;
uniform float       time;

/**
 * \brief 2D simple hash
 */
vec2 wavy_line(vec2 point)
{
    return fract(vec2(83492791.5, 19349663.0) * dot(point, vec2(7.0, 157.0)));
}
vec2 split(vec2 point)
{
    return vec2(83492791.5, 19349663.0) * dot(point, vec2(7.0, 157.0));
}

/**
 * \brief entry point
 */
void main()
{
	//get fragment to camera vector
    vec3 frag_to_cam     = vec3(rotation_matrix * vec4(normalize(position), 0.0));
    
	//create vector modifiers
    vec2 displacement = vec2(sin(time + frag_to_cam.y * 10.0) / 10.0,
                             cos(time + frag_to_cam.y * 10.0) / 10.0);
    
	//apply modifiers
    frag_to_cam.xz += displacement;

	//TODO: this is probably assigning awful values
	float split_x = step(-0.0, frag_to_cam.x);
	
    gl_FragColor = vec4(color * split_x, 1.0);
}

