#pragma once
#include "../needed_libs.h"

class DirectionalLight
{
private:
	float brightness;
	glm::vec3 rotation;
	glm::vec3 color;

	GLint brightnessLoc;
	GLint directionLoc;
	GLint colorLoc;

	Shader* program;
	
	void initUniforms();

	glm::vec3 rotationToDirection();
public:
	DirectionalLight(Shader* coreProgram,
		const glm::vec3& startRot,
		const glm::vec3& startColor,
		float startBrightness);
	~DirectionalLight();

	void render();

	inline void setRotation(const glm::vec3& newRot) 
	{ this->rotation = newRot; }

	inline void setColor(const glm::vec3& newColor)
	{
		this->color = newColor;
	}

	inline void setBrightness(float newBrightness)
	{
		this->brightness = newBrightness;
	}


};

