#version 330 core

out vec4 FragColor;

in vec4 v_Color;
in vec2 v_UV;

#define MAX_LIGHTS 16

uniform int  u_NumLights;
uniform vec2 u_LightPos[MAX_LIGHTS];
uniform vec3 u_LightColor[MAX_LIGHTS];
uniform float u_LightRadius[MAX_LIGHTS];

void main()
{
    vec4 base = v_Color;

    // Lighting
    vec3 lighting = vec3(0.0);

    for(int i = 0; i < u_NumLights; i++)
    {
        float dist = length(gl_FragCoord.xy - u_LightPos[i]);
        float att = clamp(1.0 - dist / u_LightRadius[i], 0.0, 1.0);
        lighting += u_LightColor[i] * att;
    }

    vec3 color = base.rgb + lighting * base.rgb;
    FragColor = vec4(color, base.a);
}
