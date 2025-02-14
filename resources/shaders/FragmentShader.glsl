#version 330 core

in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D diffuseTexture;

out vec4 FragColor;

void main()
{
    FragColor = texture(diffuseTexture, TexCoords);
}
