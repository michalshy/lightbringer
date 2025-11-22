#version 330 core
layout(location = 0) out vec4 FragColor;

in vec4 v_Color;
in vec2 v_TexCoord;
in vec3 v_WorldPos;

// Support up to N lights
#define MAX_LIGHTS 16

uniform int u_NumLights;
uniform vec2 u_LightPos[MAX_LIGHTS];
uniform vec3 u_LightColor[MAX_LIGHTS];
uniform float u_LightRadius[MAX_LIGHTS];

void main()
{
    vec3 ambient = vec3(0.15); // tweak ambient
    vec3 lit = v_Color.rgb * ambient;

    for (int i = 0; i < u_NumLights; i++)
    {
        float radius = max(u_LightRadius[i], 0.0001);
        float dist = length(v_WorldPos.xy - u_LightPos[i]);
        float t = clamp(dist / radius, 0.0, 1.0);
        float intensity = 1.0 - smoothstep(0.0, 1.0, t);

        lit += v_Color.rgb * u_LightColor[i] * intensity;
    }

    FragColor = vec4(lit, v_Color.a);
}