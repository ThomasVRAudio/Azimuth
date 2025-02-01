#version 460 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D g_Texture;

void main()
{
     //FragColor = vec4(vec3(1.0 - texture(g_Texture, TexCoords).rgb), 1.0);
     FragColor = texture(g_Texture, TexCoords);
}