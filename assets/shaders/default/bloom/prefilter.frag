#version 460 core
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D g_Texture;

void main() {

    vec3 color = texture(g_Texture, texCoord).rgb;

    float d = dot(color, color);

    if (d <= 2.0)
        color = vec3(0.0);

    FragColor = vec4(color, 1.0); 

}
