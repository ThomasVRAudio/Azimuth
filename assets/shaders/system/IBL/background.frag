#version 460 core
out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environmentMap;
uniform float g_Intensity;

void main()
{		
    vec3 envColor = texture(environmentMap, WorldPos).rgb;
    
    FragColor = vec4(envColor * g_Intensity, 1.0);
}