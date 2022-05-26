#include "Camera.h"

Camera::Camera(GLfloat sensitivity){
	eyePosition = glm::vec3(0.0f, 0.0f, 0.0f);
	viewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	upVector = glm::vec3(0.0f, 1.0f, 0.0f);

	SENSE = sensitivity;
	cameraSpeed = 0.5f;
}

glm::mat4 Camera::getWorldToViewMatrix(){
	return glm::lookAt(eyePosition, eyePosition + viewDirection, upVector);
}


void Camera::updateEyePosition(GLfloat leftOrBack, bool isStrafing){
	// -- leftOrBack is either -1.0f or 1.0f boof code
	glm::vec3 strafeDirection = glm::cross(viewDirection, upVector);
	eyePosition += (isStrafing) ? (cameraSpeed * strafeDirection * leftOrBack) : (cameraSpeed * viewDirection * leftOrBack);
}

void Camera::updateViewDirection(glm::vec3 delta){
	// -- view direction based on mouse
	if (glm::length(delta) < 80.0f) {
		viewDirection = glm::mat3(glm::rotate(delta.x * SENSE, upVector)) * viewDirection;
		viewDirection = glm::mat3(glm::rotate(delta.y * SENSE, glm::cross(viewDirection, upVector))) * viewDirection;
	}
	else { std::cout << "skiped" << std::endl; }
}

void Camera::updateUpVector(glm::vec3 delta){
	upVector += delta;
}


