#version 330 core
layout (location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform vec4 u_Color; // Color passed to fragment shader

out vec4 v_Color;

void main()
{
    v_Color = u_Color; // Pass color directly
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}