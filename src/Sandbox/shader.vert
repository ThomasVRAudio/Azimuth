#version 460
layout (location = 0) in vec3 aPos;

uniform mat4 g_Model;
uniform mat4 g_View;
uniform mat4 g_Projection;

out vec4 Pos;

void main() {

    gl_Position = g_Projection * g_View * g_Model * vec4(aPos, 1.0f);
    Pos = gl_Position;
}