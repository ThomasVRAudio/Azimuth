/* CREATION INSTRUCTIONS
# Vert: assets/shaders/library/default/light/light.vert
# Lit: 0
*/

#version 460 core
out vec4 FragColor;

uniform vec3 u_Color;
uniform float u_HDR;

void main() {
    FragColor = vec4(u_Color * u_HDR, 1.0); 
}