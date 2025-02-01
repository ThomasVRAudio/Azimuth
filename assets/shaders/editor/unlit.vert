#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 g_Model;
uniform mat4 g_View;
uniform mat4 g_Projection;
uniform int g_Entity;

out flat int Entity;

void main()
{
    Entity = g_Entity;
    gl_Position = g_Projection * g_View * g_Model * vec4(aPos, 1.0);
}