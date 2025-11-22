#version 330 core

in vec4 v_Color;
in vec2 v_TexCoord;
flat in int v_TexIndex;

uniform sampler2D u_Textures[16];

out vec4 OutColor;

void main()
{
    vec4 texColor = texture(u_Textures[v_TexIndex], v_TexCoord);
    OutColor = v_Color * texColor;
}