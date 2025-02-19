#include "DirectionalLight.h"

void DirectionalLight::initUniforms()
{
	this->program->use();
	this->brightnessLoc = glGetUniformLocation(this->program->getID(), "dirLight.brightness");
	this->directionLoc = glGetUniformLocation(this->program->getID(), "dirLight.direction");
	this->colorLoc = glGetUniformLocation(this->program->getID(), "dirLight.color");
	this->program->unuse();
}

glm::vec3 DirectionalLight::rotationToDirection()
{
	glm::vec3 radRotation = glm::radians(this->rotation);
	glm::vec3 direction;

	direction.x = cos(radRotation.x) * cos(radRotation.y);
	direction.y = sin(radRotation.y);
	direction.z = sin(radRotation.x) * cos(radRotation.y);

	return glm::normalize(direction);
}

DirectionalLight::DirectionalLight(Shader* coreProgram,
	const glm::vec3& startRot, const glm::vec3& startColor,
	float startBrightness)
	: program(coreProgram), rotation(startRot), color(startColor),
	brightness(startBrightness)
{
	this->initUniforms();
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::render()
{
	this->program->use();
	glUniform1f(this->brightnessLoc, this->brightness);
	glUniform3fv(this->directionLoc, 1, glm::value_ptr(rotationToDirection()));
	glUniform3fv(this->colorLoc, 1, glm::value_ptr(this->color));
	this->program->unuse();
}
