#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <GLM/glm.hpp>
#include <GLM/vec2.hpp>
#include <GLM/vec3.hpp>
#include <GLM/vec4.hpp>
#include <GLM/mat4x4.hpp>
#include <GLM/ext/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <SOIL2/SOIL2.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texcoord;
	glm::vec3 normal;
};

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

#include "Window.h"
#include "Camera.h"
#include "Shader.h"