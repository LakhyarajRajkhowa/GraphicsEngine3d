#version 330 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Color;

uniform mat4 u_ViewProj;

out vec3 v_Color;

void main()
{
    v_Color     = a_Color;
    gl_Position = u_ViewProj * vec4(a_Pos, 1.0);
}
