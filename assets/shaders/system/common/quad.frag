#version 460 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D g_Texture;

void main()
{
     FragColor = texture(g_Texture, TexCoords);
}

