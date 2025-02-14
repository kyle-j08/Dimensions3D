#include "Camera.h"

Camera::Camera(float fov, float nearView, float farView,
	glm::vec3 startPosition, glm::vec3 startRotation, float aspectRatio)
	:	projectionMatrix(1.f), viewMatrix(1.f), worldUp(glm::vec3(0.f,1.f,0.f)),
	fieldOfView(fov), cameraPosition(startPosition), cameraRotation(startRotation),
	nearClipping(nearView), farClipping(farView)
{
	this->updateMatrixes(aspectRatio);
}

Camera::~Camera()
{

}



void Camera::updateMatrixes(float aspectRatio)
{
	this->viewMatrix = glm::mat4(1.f);
	this->projectionMatrix = glm::mat4(1.f);
	cameraRotation.x = glm::clamp(cameraRotation.x, -89.9f, 89.9f);

	glm::vec3 front;
	front.x = cos(glm::radians(cameraRotation.y)) * cos(glm::radians(cameraRotation.x));
	front.y = sin(glm::radians(cameraRotation.x));
	front.z = -sin(glm::radians(cameraRotation.y)) * cos(glm::radians(cameraRotation.x));
	front = glm::normalize(front);

	this->viewMatrix = glm::lookAt(this->cameraPosition, this->cameraPosition - front, this->worldUp);


	this->projectionMatrix = glm::perspective(glm::radians(this->fieldOfView),
		aspectRatio, this->nearClipping, this->farClipping);
}

