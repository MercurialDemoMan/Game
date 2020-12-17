#version 120

varying vec2 coord;

uniform vec2 dims;
uniform sampler2D sampler;

uniform float blur_strength;

float sCurve (float x) {
    // ---- by CeeJayDK
    x = x * 2.0 - 1.0;
    return -x * abs(x) * 0.5 + x + 0.5;
}

vec4 blurH (sampler2D source, vec2 size, vec2 uv, float radius) {
    
    if (radius >= 1.0)
    {
        vec3 C = vec3(0.0);
        
        float width = 1.0 / size.x;
        
        float divisor = 0.0;
        float weight = 1.0;
        
        float radiusMultiplier = 1.0 / radius * (size.y / size.x);
        
        for (float x = -radius; x <= radius; x += radius / 2.0)
        {
            //weight = sCurve(1.0 - (abs(x) * radiusMultiplier));
            C += texture2D(source, uv + vec2(x * width, 0.0)).rgb * weight;
            divisor += weight;
        }
        
        return vec4(C.rgb / divisor, 1.0);
    }
    
    return texture2D(source, uv);
}

void main() {
    gl_FragColor = blurH(sampler, dims, coord, blur_strength);
}
