#pragma once
#include "../needed_libs.h"

class Camera
{
private:
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::vec3 cameraPosition;
	glm::vec3 cameraRotation;
	const glm::vec3 worldUp;
	float fieldOfView;
	float nearClipping;
	float farClipping;


public:
	Camera(float fov, float nearView = 0.1f, float farView = 200.f,
		glm::vec3 startPosition = glm::vec3(0.f), glm::vec3 startRotation = glm::vec3(0.f), float aspectRatio = 1.778f);
	~Camera();


	inline glm::mat4 getViewMatrix() const {return this->viewMatrix;}
	inline glm::mat4 getProjectionMatrix() const {return this->projectionMatrix;}
	void updateMatrixes(float aspectRatio);

	inline glm::vec3 getPosition() const { return this->cameraPosition; }
	inline glm::vec3 getRotation() const {return this->cameraRotation;}

	inline void setPosition(glm::vec3 newPos) {this->cameraPosition = newPos;}
	inline void setRotation(glm::vec3 newRot) {this->cameraRotation = newRot;}


	inline void addRotation(glm::vec3 toAdd) {this->cameraRotation += toAdd;}
	inline void addPosition(glm::vec3 toAdd) {this->cameraPosition += toAdd;}

};

