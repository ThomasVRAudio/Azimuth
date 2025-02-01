#version 460 core
in vec4 Pos;

out vec4 FragColor;

void main() {
    vec3 result = vec3(Pos.x, Pos.y, Pos.z) * 0.5 + 0.5;
    FragColor = vec4(result, 1.0);
}