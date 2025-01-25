#version 330 core
out vec4 FragColor;

in vec3 TexCoords; 
uniform samplerCube g_Skybox; 

void main()
{
    FragColor = texture(g_Skybox, TexCoords);
}