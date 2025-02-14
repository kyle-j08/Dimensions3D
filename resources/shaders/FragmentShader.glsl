#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;



out vec4 FragColor;

void main()
{
    vec3 ambientFinal = light.ambient * vec3(texture(material.diffuse, TexCoords));

    //Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, light.position), 0.0);
    vec3 diffuseFinal = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    //Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specularFinal = light.specular * spec * vec3(texture(material.specular, TexCoords));

    

    vec3 result = ambientFinal + diffuseFinal + specularFinal;
    FragColor = vec4(result, 1.0);
}
