#version 120

varying vec3 position;
varying vec3 normal;
varying vec2 coord;

uniform sampler2D text_sampler;

uniform float g_time;
uniform float shade_time;
uniform vec2 dims;

uniform float pixel_factor;
uniform float volume;
uniform bool effects;
uniform bool text_mode;
uniform vec2 shown_text;
uniform vec2 mouse_pos;

#define VOL_BAR_WIDTH (dims.x * 4.0 / 5.0 - dims.x * 1.0 / 5.0)
#define VOL_BAR_START_X (dims.x * 1.0 / 5.0)

float rectangle(vec2 uv, vec2 pos, float width, float height, float blur) {
    pos = (vec2(width, height) + 0.01) / 2.0 - abs(uv - pos);
    pos = smoothstep(0.0, blur, pos);
    return pos.x * pos.y;
}

float speaker(vec2 uv, vec2 pos, float width, float height) {
    float res = 0.0;
    if(volume > 0.0) {
            res += rectangle(uv, pos - vec2(width / 2.0, 0.0), width * 0.1, height * 0.2, 0.0);
    }
    if(volume > 1.0 / 3.0) {
        res += rectangle(uv, pos + vec2(0.0, 0.0), width * 0.1, height * 0.5, 0.0);
    }
    if(volume > 2.0 / 3.0) {
        res += rectangle(uv, pos + vec2(width / 2.0, 0.0), width * 0.1, height * 0.8, 0.0);
    }
    return res;
}

float sphere(vec2 uv, vec2 pos, float r) {
    vec2 d = pos - uv;
    return 1.0 - (smoothstep(r*r, r*r + 30.0, dot(d, d)));
}

vec4 getText(vec2 uv) {
    if(uv.x <= shown_text.x && uv.y <= shown_text.y) {
        return texture2D(text_sampler, uv - vec2(0.0, -0.3) * (float(text_mode) / 2.0 + 0.5));
    }
    return vec4(0.0);
}


void main() {
    //pixelation
    //vec2 pix = coord * dims;
    vec2 pix = ceil(coord * dims / pixel_factor) * pixel_factor;
    vec2 uv  = pix / dims;
    
    vec4 FragColor = vec4(0.0);
    
    if(text_mode) {
        FragColor = getText(uv);
    } else {
        //text
        if(rectangle(mouse_pos, (dims / 2.0) + vec2(0.0, dims.y * 0.15), dims.x / 2.7, dims.y / 10.0, 0.0) != 0.0) {
            FragColor = getText(uv + vec2(0.0, sin(uv.x * 20.0 + g_time * 10.0) / 30.0));
        } else {
            FragColor = getText(uv);
        }
        
        //volume bar
        float threshold = VOL_BAR_WIDTH * volume;
        if(rectangle(pix, vec2(dims.x / 2.0, dims.y * 19.0 / 20.0), VOL_BAR_WIDTH, pixel_factor, 0.0) != 0.0) {
            if(pix.x > VOL_BAR_START_X + threshold) {
                FragColor.a += 0.25;
            } else {
                FragColor.a += 0.5;
            }
            FragColor.rgb += 1.0;
        }
        
        //volume circle
        if(sphere(pix, vec2(VOL_BAR_START_X + threshold, dims.y * 19.0 / 20.0), 7.25) != 0.0) {
            FragColor = vec4(1.0, 1.0, 1.0, 0.5) *
                        sphere(pix, vec2(VOL_BAR_START_X + threshold, dims.y * 19.0 / 20.0), 7.25) *
                 (1.0 - sphere(pix, vec2(VOL_BAR_START_X + threshold, dims.y * 19.0 / 20.0), 4.25));
        }
        
        //volume symbol
        FragColor += speaker(pix, vec2(dims.x / 2.0, dims.y * 9.0 / 10.0), 20.0, 20.0);
        
        //effect symbol
        if(effects) {
            FragColor += vec4(0.3, 0.5, 0.7, 1.0) * sphere(pix, vec2(dims.x / 2.0, dims.y * 1.0 / 7.0), 10.0);
            
            if(FragColor.a > 0.1) {
                FragColor.rgb += vec3(0.05, 0.25, 0.3) * sphere(pix, vec2(dims.x * 0.495, dims.y * 1.0 / 6.75), 10.0);
            }
            
            FragColor.rgb += sphere(pix, vec2(dims.x * 0.495, dims.y * 1.0 / 6.75), 2.0);
        } else {
            FragColor += vec4(1.0, 1.0, 1.0, 1.0) * sphere(pix, vec2(dims.x / 2.0, dims.y * 1.0 / 7.0), 10.0);
        }
    }
    
    //final color
    gl_FragColor = vec4(FragColor.rgb * shade_time, FragColor.a + (float(!text_mode) * 0.25));
}
