#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirLight g_DirLight;
uniform vec3 g_ViewPos;

uniform Material u_Material;
uniform vec3 u_Color;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.diffuse * spec;

    return (ambient + diffuse + specular);
};

void main()
{    
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(g_ViewPos - FragPos);

    vec3 result = CalcDirLight(g_DirLight, normal, viewDir);
    result *= u_Color;
    FragColor = vec4(result, 1.0f); 
}