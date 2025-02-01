#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 g_Model;
uniform mat4 g_View;
uniform mat4 g_Projection;

void main()
{
    TexCoords = aTexCoords;    
    FragPos = vec3(g_Model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(g_Model))) * aNormal;
    gl_Position = g_Projection * g_View * g_Model * vec4(aPos, 1.0);
}