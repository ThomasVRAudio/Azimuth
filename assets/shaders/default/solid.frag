#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 g_LightPos;
uniform vec3 g_LightColor;
uniform vec3 g_ViewPos;
uniform vec3 u_Color;

void main()
{    
    float shininess = 32.0f;
    vec3 normal = normalize(Normal);

    vec3 lightDir = normalize(g_LightPos - FragPos);
    vec3 viewDir = normalize(g_ViewPos - FragPos);

    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

    float distance = length(g_LightPos - FragPos);
    float attenuation = 1.0 / (1.0f + 0.09f * distance + 0.0032f * (distance * distance)); 

    vec3 diffuse = vec3(0.8f, 0.8f, 0.8f) * diff * attenuation * u_Color; 
    vec3 specular = vec3(1.0f, 1.0f, 1.0f) * spec * attenuation;
    vec3 ambient = vec3(0.2f, 0.2f, 0.2f) * u_Color;  

    vec3 result = (ambient + specular + diffuse) * g_LightColor;
    FragColor = vec4(result, 1.0f); 
}