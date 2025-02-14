#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexcoord;
layout(location = 2) in vec3 vertexNormal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec2 TexCoords;
out vec3 FragPos;

void main()
{
    FragPos = vec3(ModelMatrix * vec4(vertexPosition, 1.0));
    TexCoords = vertexTexcoord;
    gl_Position = ProjectionMatrix * ViewMatrix * vec4(FragPos, 1.0);
}
