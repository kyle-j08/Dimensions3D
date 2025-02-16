#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
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
uniform Material material;
uniform Light light;

out vec4 FragColor;

void main()
{
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(-light.direction));

    float intensity = 0.0;

    if (theta > light.outerAngle) {
        if (theta > light.innerAngle) {
            intensity = 1.0; // Inside inner cone
        } else {
            float smoothFactor = (theta - light.outerAngle) / (light.innerAngle - light.outerAngle);
            intensity = clamp(smoothFactor, 0.0, 1.0);
        }
    }

    float distance = length(light.position - FragPos);
    float attenuation = clamp(1.0 - (distance / light.fallOffDistance), 0.0, 1.0);
    attenuation *= attenuation;

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    diffuse *= light.brightness * attenuation * intensity;
    specular *= light.brightness * attenuation * intensity;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
