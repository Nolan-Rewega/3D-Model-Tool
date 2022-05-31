#include "Camera.h"

Camera::Camera(GLfloat sensitivity, GLfloat givenRadius){
	radius = givenRadius;
	theta = glm::radians(90.0f);
	phi = glm::radians(90.0f);

	offset = glm::vec3(0.0, 0.0, -3.0f);


	eyePosition = glm::vec3(0.0f, 0.0f, 0.0f);
	viewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	upVector = glm::vec3(0.0f, 1.0f, 0.0f);

	SENSE = sensitivity;
	cameraSpeed = 0.5f;
	cycle = 0;
}

glm::mat4 Camera::getWorldToViewMatrix(){
	return glm::lookAt(eyePosition, eyePosition + viewDirection, upVector);
}

void Camera::sphereRotation(GLfloat dTheta, GLfloat dPhi) {
	// -- Weird code.
	// -- fixing the openGL camera by fliping the up vector whenever phi 
	// -- equals a multiple of pi. This also requires me to swap how dTheta
	// -- is added to maintain horizontal camera control.

	if (abs(cycle) % 2 == 0) { theta -= dTheta * SENSE; }
	else { theta += dTheta * SENSE; }

	phi += dPhi * SENSE;
	phi = roundf(phi * 100.0f) / 100.0f;

	if ( phi > roundf((cycle+1)*3.1415f * 10000.0f) / 10000.0f) {
		upVector = -upVector;
		cycle++;
	}
	else if (phi < roundf(cycle * 3.1415f * 10000.0f) / 10000.0f) {
		upVector = -upVector;
		cycle--;
	}

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
	GLfloat y = radius * glm::cos(phi);

	eyePosition = glm::vec3(
		radius * glm::sin(phi) * glm::cos(theta),
		radius * glm::cos(phi),
		radius * glm::sin(phi) * glm::sin(theta)
	) + offset;

	// -- view direction is the normal vector of sphere surface
	viewDirection = -eyePosition + offset;

}



