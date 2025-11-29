#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;
in vec4 v_Color;
in vec3 v_WorldPos;

#define MAX_LIGHTS 16

uniform sampler2D u_Texture;
uniform int   u_NumLights;
uniform vec2  u_LightPos[MAX_LIGHTS];
uniform vec3  u_LightColor[MAX_LIGHTS];
uniform float u_LightRadius[MAX_LIGHTS];

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);

    if (texColor.a <= 0.01)
        discard;

    texColor *= v_Color;

    vec3 baseColor = texColor.rgb;
    vec3 ambient   = baseColor * 0.15;
    vec3 lit       = ambient;

    for (int i = 0; i < u_NumLights; i++)
    {
        float radius = max(u_LightRadius[i], 0.0001);
        float dist = length(v_WorldPos.xy - u_LightPos[i]);

        float t = clamp(dist / radius, 0.0, 1.0);
        float intensity = 1.0 - smoothstep(0.0, 1.0, t);

        lit += baseColor * u_LightColor[i] * intensity;
    }

    FragColor = vec4(lit, texColor.a);
}
