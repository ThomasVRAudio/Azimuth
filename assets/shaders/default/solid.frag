#version 330 core
out vec4 FragColor;

uniform vec3 Color;
uniform float colorScalar;

void main()
{
    FragColor = vec4(Color * colorScalar, 1.0); 
}