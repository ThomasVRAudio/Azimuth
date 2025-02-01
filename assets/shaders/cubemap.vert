#version 460 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 g_Projection;
uniform mat4 g_View;

void main()
{
    TexCoords = aPos;
    vec4 pos = g_Projection * g_View * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}