#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D g_HdrBuffer;
uniform float g_Exposure;

void main()
{
    vec3 hdrColor = texture(g_HdrBuffer, TexCoords).rgb;

    vec3 toneMapped = vec3(1.0) - exp(-hdrColor * g_Exposure);

    toneMapped = pow(toneMapped, vec3(1.0 / 2.2));
    FragColor = vec4(toneMapped, 1.0);
}