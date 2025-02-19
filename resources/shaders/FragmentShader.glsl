#version 330 core
#define NR_POINT_LIGHTS 4
#define NR_SPOT_LIGHTS 4

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    int useSpecular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
    vec3 color;
    float brightness;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float brightness;
    float falloffDistance;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    float brightness;
    float innerAngle;
    float outerAngle;
    float falloffDistance;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir); 
        
    for(int i = 0; i < NR_SPOT_LIGHTS; i++)
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);    
    
    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 diffuse = light.color * diff * vec3(texture(material.diffuse, TexCoords));
    if(material.useSpecular == 1)
    {
        vec3 specular = vec3(texture(material.specular, TexCoords));
        specular = light.color * spec * specular;
        return (diffuse + specular) * light.brightness;
    }
    else
    {
        return diffuse * light.brightness;
    }


}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = (1.0 - (distance / light.falloffDistance));
    attenuation = attenuation * attenuation; // Makes falloff smoother
    attenuation = clamp(attenuation, 0.0, 1.0);

    vec3 diffuse = light.color * diff * vec3(texture(material.diffuse, TexCoords));
    if(material.useSpecular == 1)
    {
        vec3 specular = vec3(texture(material.specular, TexCoords));
        specular = light.color * spec * specular;
        diffuse *= attenuation * light.brightness;
        specular *= attenuation * light.brightness;
        return diffuse + specular;
    }
    else
    {
        diffuse *= attenuation * light.brightness;
        return diffuse;
    }


}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float distance = length(light.position - fragPos);
    float attenuation = (1.0 - (distance / light.falloffDistance));

    attenuation = attenuation * attenuation; // Makes falloff smoother
    attenuation = clamp(attenuation, 0.0, 1.0);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerAngle - light.outerAngle;
    float intensity = clamp((theta - light.outerAngle) / epsilon, 0.0, 1.0);


    vec3 diffuse = light.color * diff * vec3(texture(material.diffuse, TexCoords));

    if(material.useSpecular == 1)
    {
        vec3 specular = vec3(texture(material.specular, TexCoords));
        specular = light.color * spec * specular;
        diffuse *= attenuation * intensity * light.brightness;
        specular *= attenuation * intensity * light.brightness;
        return diffuse + specular;
    }
    else
    {
        diffuse *= attenuation * intensity * light.brightness;
        return diffuse;
    }


    
}
