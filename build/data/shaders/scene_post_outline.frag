#version 120

#define EdgeStep 2.0

varying vec3 position;
varying vec3 normal;
varying vec2 coord;

uniform sampler2D sampler;

uniform vec2 dims;
uniform vec3 edge_color;

float lookup(vec2 p, float dx, float dy) 
{
    vec2 uv = (p.xy + vec2(dx * EdgeStep, dy * EdgeStep)) / dims.xy;
    vec4 c  = texture2D(sampler, uv);
    return 0.288 * c.r + 0.587 * c.g + 0.114 * c.b;
}

void main() {

    //edge detection
    vec2 p = coord * dims;
    
    vec2 g = vec2(0.0);
    g.x += -1.0 * lookup(p, -1.0, -1.0);
    g.x += -2.0 * lookup(p, -1.0,  0.0);
    g.x += -1.0 * lookup(p, -1.0,  1.0);
    g.x +=  1.0 * lookup(p,  1.0, -1.0);
    g.x +=  2.0 * lookup(p,  1.0,  0.0);
    g.x +=  1.0 * lookup(p,  1.0,  1.0);
    
    g.y += -1.0 * lookup(p, -1.0, -1.0);
    g.y += -2.0 * lookup(p,  0.0, -1.0);
    g.y += -1.0 * lookup(p,  1.0, -1.0);
    g.y +=  1.0 * lookup(p, -1.0,  1.0);
    g.y +=  2.0 * lookup(p,  0.0,  1.0);
    g.y +=  1.0 * lookup(p,  1.0,  1.0);
	
    //final color
    gl_FragColor = texture2D(sampler, coord) + vec4(edge_color.rgb * dot(g, g), 0.0);
}
