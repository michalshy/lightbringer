#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

layout(location = 2) in vec4 a_InstanceMatCol0;
layout(location = 3) in vec4 a_InstanceMatCol1;
layout(location = 4) in vec4 a_InstanceMatCol2;
layout(location = 5) in vec4 a_InstanceMatCol3;

layout(location = 6) in vec4 a_InstanceColor;
layout(location = 7) in vec4 a_UVRect;    // (u0, v0, scaleU, scaleV)

out vec2 v_TexCoord;
out vec4 v_Color;
out vec3 v_WorldPos;

uniform mat4 u_ViewProjection;

void main()
{
    mat4 M = mat4(
        a_InstanceMatCol0,
        a_InstanceMatCol1,
        a_InstanceMatCol2,
        a_InstanceMatCol3
    );

    vec4 worldPos = M * vec4(a_Position, 1.0);

    gl_Position = u_ViewProjection * worldPos;

    v_TexCoord = a_TexCoord * a_UVRect.zw + a_UVRect.xy;

    v_Color = a_InstanceColor;
    v_WorldPos = worldPos.xyz;
}
