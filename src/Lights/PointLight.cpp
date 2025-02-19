#include "PointLight.h"

void PointLight::initUniforms()
{
	this->program->use();
	std::string base = "pointLights[" + std::to_string(lightNum) + "]";

	positionLoc = glGetUniformLocation(program->getID(), (base + ".position").c_str());
	colorLoc = glGetUniformLocation(program->getID(), (base + ".color").c_str());
	brightnessLoc = glGetUniformLocation(program->getID(), (base + ".brightness").c_str());
	fallOffLoc = glGetUniformLocation(program->getID(), (base + ".falloffDistance").c_str());

	this->program->unuse();
}

PointLight::PointLight(Shader* coreProgram, const glm::vec3& startPos,
	const glm::vec3& startColor, float startBrightness,
	float startFallOff)
	: position(startPos), color(startColor), brightness(startBrightness),
	fallOff(startFallOff), program(coreProgram), positionLoc(0),
	colorLoc(0), brightnessLoc(0), fallOffLoc(0), lightNum(0)
{
	initUniforms();
}

PointLight::~PointLight()
{
}

void PointLight::render()
{
	this->program->use();

	glUniform3fv(positionLoc, 1, glm::value_ptr(position));
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));
	glUniform1f(brightnessLoc, brightness);
	glUniform1f(fallOffLoc, fallOff);

	this->program->unuse();
}
