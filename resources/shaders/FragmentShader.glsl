#version 330 core
#define MAX_POINT_LIGHTS 10
#define MAX_SPOT_LIGHTS 10

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

    float brightness;

};

struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float brightness;
    float fallOffDistance;
    
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float brightness;
    float innerAngle;
    float outerAngle;
    float fallOffDistance;
};

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform Material material;



out vec4 FragColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);


    vec3 result = CalcDirLight(dirLight, norm, viewDir);


    for(int i = 0; i < MAX_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

    for(int i = 0; i < MAX_SPOT_LIGHTS; i++)
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);


    FragColor = vec4(result, 1.0);

}



vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    //Diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    //Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir),0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular) * light.brightness; 
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    //Diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    //Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    //FallOff
    float distance = length(light.position - fragPos);
    float attenuation = clamp(1.0 - (distance / light.fallOffDistance), 0.0, 1.0);   


    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular) * light.brightness;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    //Diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    //Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    //FallOff
    float distance = length(light.position - fragPos);
    float attenuation = clamp(1.0 - (distance / light.fallOffDistance), 0.0, 1.0);   
    float theta = dot(lightDir, normalize(-light.direction));
    float intensity = 0.0;
    if (theta > light.outerAngle) {
        if (theta > light.innerAngle) {
            intensity = 1.0;
        } else {
            float smoothFactor = (theta - light.outerAngle) / (light.innerAngle - light.outerAngle);
            intensity = clamp(smoothFactor, 0.0, 1.0);
        }
    }

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    ambient *= light.brightness * attenuation * intensity;
    diffuse *= light.brightness * attenuation * intensity;
    specular *= light.brightness * attenuation * intensity;

    return (ambient + diffuse + specular);
}
