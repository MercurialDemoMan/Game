#version 120

varying vec3 position;
varying vec3 normal;
varying vec3 coord;
varying vec3 l_p;

uniform vec3 m_ambient;	//gl_FrontMaterial
uniform vec3 m_diffuse;
uniform vec3 m_specular;
uniform float m_alpha;
uniform float shininess;

uniform vec3 l_ambient;	//gl_LightSource[0]
uniform vec3 l_diffuse;
uniform vec3 l_specular;

uniform vec2 dims;
uniform float g_time;
uniform vec3 status_color;
uniform float reflection_strength;

uniform samplerCube cube_map;
uniform mat4 rot_mat;

uniform vec3 obj_pos;

uniform float dark_mode;

#define PI 3.14159

#define FOG_CONST 0.06
#define FOG_COLOR vec3(0.01, 0.2, 0.4)

float hash(vec3 p) {
    p  = fract( p*0.3183099+.1 );
    p *= 17.0;
    return fract( p.x*p.y*p.z*(p.x+p.y+p.z) );
}

float noise(vec3 x)
{
    vec3 p = floor(x);
    vec3 f = fract(x);
    f = f * f * (3.0 - 2.0 * f);

    return mix(mix(mix(hash(p + vec3(0, 0, 0)),
                       hash(p + vec3(1, 0, 0)), f.x),
                   mix(hash(p + vec3(0, 1, 0)),
                       hash(p + vec3(1, 1, 0)), f.x), f.y),
               mix(mix(hash(p + vec3(0, 0, 1)),
                       hash(p + vec3(1, 0, 1)), f.x),
                   mix(hash(p + vec3(0, 1, 1)),
                       hash(p + vec3(1, 1, 1)), f.x), f.y), f.z);
}

/*vec3 applyFog(vec3 rgb,     // original color of the pixel
              float dist)   // camera to point distance
{
    float fogAmount = 1.0 - exp(-dist * FOG_CONST);
    return mix(rgb, FOG_COLOR, fogAmount);
}*/

void main()
{    
	//gl_FragColor = vec4(m_diffuse, 1.0);
	//return;

    //////LIGHTING
    float dist   = length(position - l_p);
    float att    = 1.0 / (1.0 + 0.01 * dist + 0.01 * dist * dist);
    vec3 ambient = m_ambient * l_ambient;
    
    vec3 surf2light = normalize(l_p - position);
    vec3 norm       = normalize(normal);
    
    float dcont = max(0.0, dot(norm, surf2light));
    //toon shading
    //float level = floor(dcont * 6.0);
    //dcont = level / 6.0;
    //////////////
    vec3 diffuse = dcont * (m_diffuse * l_diffuse);
    
    vec3 surf2view  = normalize(-position);
    vec3 reflection = reflect(-surf2light, norm);
    
    float scont = pow(max(0.0, dot(surf2view, reflection)), shininess);
    //toon shading
    //level = floor(scont * 6.0);
    //scont = level / 6.0;
    //////////////
    vec3 specular = scont * l_specular * m_specular;
    
    vec4 col = vec4(ambient + diffuse + specular * att, m_alpha);
    
    if(shininess == -1.0) { col = vec4(m_diffuse, m_alpha); }
    
    //plasma indication color
    /*float t = length(obj_pos) + g_time / 2.0;
    vec3 oa = vec3(t, 0.2 + t, -t);
    vec3 ob = vec3(-t, t * 0.5, t);
    float a = noise(coord + oa), b = noise(coord + ob);
    float c = 1.0 - pow(abs(b - a), 1.0 / 5.0);
    vec4  q = vec4(c * status_color, 1.0);*/
    
    //reflection manipulation
    vec3 ref    = reflect(-surf2view, norm); ref = vec3(rot_mat * vec4(ref, 0.0));
    vec3 ref2   = ref;
    float sine  = sin(g_time + ref.y * 10.0) / 10.0;
    float csine = cos(g_time + ref.y * 10.0) / 10.0;
    
    ref.x  += sine;
    ref.z  += csine;
    ref2.x -= sine;
    ref2.z -= csine;
    
    gl_FragColor = col +
            (vec4(textureCube(cube_map, ref).rgb / 5.0 +
                  textureCube(cube_map, ref2).rgb / 16.0, 0.0) * dcont * reflection_strength)/* + q*/;
    
    gl_FragColor.rgb *= dark_mode;
}
