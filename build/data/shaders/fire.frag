#version 120
varying vec3 position;
varying vec3 normal;
varying vec2 coord;
varying vec3 l_p;

uniform vec3 m_diffuse;

uniform float dark_mode;

void main()
{
    gl_FragColor      = vec4(m_diffuse, 1.0);
    gl_FragColor.rgb *= dark_mode;
}
