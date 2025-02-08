#version 460 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D g_Texture1;
uniform sampler2D g_Texture2;
uniform float g_Blend;

void main() {

    vec3 color1 = texture(g_Texture1, TexCoords).rgb;
    vec3 color2 = texture(g_Texture2, TexCoords).rgb;

    FragColor = vec4(mix(color1, color2, g_Blend), 1.0); 
}