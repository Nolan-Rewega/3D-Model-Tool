#include "Camera.h"

Camera::Camera(GLfloat sensitivity, GLfloat givenRadius){
	radius = givenRadius;
	theta = glm::radians(0.0f);
	phi = glm::radians(3.1415f / 2.0f);

	offset = glm::vec3(0.0, 0.0, -3.0f);


	eyePosition = glm::vec3(0.0f, 0.0f, 0.0f);
	viewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	upVector = glm::vec3(0.0f, 1.0f, 0.0f);

	SENSE = sensitivity;
	cameraSpeed = 0.5f;
}

glm::mat4 Camera::getWorldToViewMatrix(){
	return glm::lookAt(eyePosition, eyePosition + viewDirection, upVector);
}

void Camera::sphereRotation(GLfloat dTheta, GLfloat dPhi) {
	theta -= dTheta * SENSE;
	phi += dPhi * SENSE;

	updateParameters();
}

void Camera::zoom(GLfloat dZoom){
	radius -= dZoom;
	updateParameters();
}

void Camera::sphereTranslate(glm::vec3 delta){
	offset += delta;
	updateParameters();
}

void Camera::updateParameters(){
	eyePosition = glm::vec3(
		radius * glm::sin(phi) * glm::cos(theta),
		radius * glm::sin(phi) * glm::sin(theta),
		radius * glm::cos(phi)
	) + offset;

	viewDirection = -eyePosition + offset; // -- normal to sphere surface
	upVector = (-eyePosition / radius) + offset;
}



