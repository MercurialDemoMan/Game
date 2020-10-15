#version 120

/**
 * interpolated vertex attributes
 */
varying vec3 position;
varying vec3 normal;
varying vec3 coord;
varying vec2 uv;

/**
 * surface material specification
 */
uniform vec3      obj_pos;
uniform vec3      material_ambient;
uniform vec3      material_diffuse;
uniform vec3      material_specular;
uniform float     material_shininess;
uniform float     reflection_strength;
uniform bool      has_uv_map;
uniform sampler2D uv_map;

/**
 * lights material specification
 */
#define NumLights 16 

uniform bool light_enabled;
uniform vec3 light_pos;
uniform vec3 light_ambient;
uniform vec3 light_diffuse;
uniform vec3 light_specular;

/**
 * game internals
 */
uniform vec2 dims;
uniform float time;
uniform samplerCube skybox;

/**
 * camera matricies
 */
uniform mat4 rot_mat;
uniform mat4 view_mat;


float hash(vec3 p) {
    p  = fract(p * 0.3183099 + 0.1) * 17.0;
    return fract(p.x * p.y * p.z * (p.x + p.y + p.z));
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

vec4 plasma()
{
	float t = length(obj_pos) + time;
    vec3 oa = vec3(t, 0.2 + t, -t);
    vec3 ob = vec3(-t, t * 0.5, t);
    float a = noise(coord + oa), b = noise(coord + ob);
    float c = 1.0 - pow(abs(b - a), 1.0 / 5.0);
    return vec4(c * vec3(0.6, 0.6, 1.0), 1.0);
}

/**
 * entry point
 */
void main()
{
	/* calculate light position */
	vec3 light = vec3(view_mat * vec4(light_pos, 1.0));

    /* calculate ambient color */
    float dist   = length(position - light);
    float att    = 1.0 / (1.0 + 0.01 * dist + 0.01 * dist * dist);
	float att2   = 1.0 / (1.0 + 0.0001 * dist + 0.0001 * dist * dist);
    vec3 ambient = material_ambient * light_ambient;
    
    vec3 surf2light = normalize(light - position);
    vec3 norm       = normalize(normal);
    
    float dcont = max(0.0, dot(norm, surf2light));
   
    /* calcualate diffuse color */
    vec3 diffuse = dcont * (material_diffuse * light_diffuse);
    
    vec3 surf2view  = normalize(-position);
    vec3 reflection = reflect(-surf2light, norm);
    
    float scont = pow(max(0.0, dot(surf2view, reflection)), dist + material_shininess);
    
	/* calculate specular color */
    vec3 specular = scont * light_specular * material_specular;
    
    vec4 col = vec4(ambient + diffuse * att2 + specular * att, 1.0);
    
    //plasma indication color
    //vec4 q = plasma();
    
    //reflection manipulation
	/*vec3 ref    = reflect(-surf2view, norm); 
	ref = vec3(rot_mat * vec4(ref, 0.0));
    vec3 ref2   = ref;
    float sine  = sin(time + ref.y * 10.0) / 10.0;
    float csine = cos(time + ref.y * 10.0) / 10.0;
    
    ref.x  += sine;
    ref.z  += csine;
    ref2.x -= sine;
    ref2.z -= csine;
	*/
	if(has_uv_map)
	{
		 gl_FragColor = col * texture2D(uv_map, uv);
	}
	else
	{
		gl_FragColor = col;
	}

	//gl_FragColor += (vec4(textureCube(skybox, ref).rgb / 5.0 + textureCube(skybox, ref2).rgb / 16.0, 0.0) * dcont * reflection_strength);
	//gl_FragColor += q;
  }
