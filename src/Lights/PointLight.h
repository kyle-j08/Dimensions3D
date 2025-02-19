#pragma once
#include "../needed_libs.h"

class PointLight
{
private:
	glm::vec3 position;
	glm::vec3 color;
	float fallOff;
	float brightness;

	GLint positionLoc;
	GLint colorLoc;
	GLint fallOffLoc;
	GLint brightnessLoc;

	Shader* program;

	int lightNum;

	void initUniforms();
public:

	PointLight(Shader* coreProgram, const glm::vec3& startPos,
		const glm::vec3& startColor, float startBrightness, float startFallOff);

	~PointLight();

	void render();

};

