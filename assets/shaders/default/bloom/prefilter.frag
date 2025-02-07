#version 460 core
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D g_Texture;
uniform float g_Threshold;

float lerp(float a, float b, float t) {
    return a + (b - a) * t;
};

void main() {

    vec3 color = texture(g_Texture, texCoord).rgb;

    float brightness = max(color.r, max(color.g, color.b));
    float contribution = max ( 0, brightness - g_Threshold);
    contribution /= max(brightness, 0.00001);
    color *= contribution;

    FragColor = vec4(color, 1.0); 



}
