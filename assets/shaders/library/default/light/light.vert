#version 460 core
layout (location = 0) in vec3 aPos;

out vec3 FragPos;

uniform mat4 g_Model;
uniform mat4 g_View;
uniform mat4 g_Projection;

void main()
{
    FragPos = vec3(g_Model * vec4(aPos, 1.0));
    gl_Position = g_Projection * g_View * g_Model * vec4(aPos, 1.0);
}