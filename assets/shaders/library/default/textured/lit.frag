/* CREATION INSTRUCTIONS
# Vert: assets/shaders/library/default/model.vert
# Lit: 1
*/

#version 460 core
layout (location = 0) out vec4 FragColor; 

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

#define MAX_POINT_LIGHTS 10


struct Material {
    sampler2D texture_diffuse;
    sampler2D texture_specular;
    sampler2D texture_normal;
    float shininess;
}; 

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

uniform DirLight g_DirLight;
uniform int g_NumPointLights;

uniform PointLight g_PointLights[MAX_POINT_LIGHTS];

uniform vec3 g_ViewPos;

uniform Material u_Material;
uniform vec3 u_Color;
uniform float u_HDR;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

    vec3 ambient = light.ambient * texture(texture_diffuse1, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;
    vec3 specular = light.diffuse * spec * texture(texture_specular1, TexCoords).rgb;

    return (ambient + diffuse + specular);
};

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords)).rgb;
    vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;
    vec3 specular = light.diffuse * spec * texture(texture_specular1, TexCoords).rgb;

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    diffuse *= attenuation;
    specular *= attenuation;
    ambient *= attenuation;

    return (ambient + diffuse + specular);
};

void main()
{    
    vec3 normal = texture(texture_normal1, TexCoords).rgb;
    vec3 viewDir = normalize(g_ViewPos - FragPos);

    vec3 result = CalcDirLight(g_DirLight, normal, viewDir);

    for (int i=0; i < g_NumPointLights; ++i)
    {
        result += CalcPointLight(g_PointLights[i], normal, FragPos, viewDir);
    };

    result *= u_HDR;
   
    FragColor = vec4(result, 1.0f); 
}