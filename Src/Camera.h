#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


class Camera {
public:

	GLfloat SENSE, cameraSpeed;

	glm::vec3 eyePosition;
	glm::vec3 viewDirection; // -- must be normalized
	glm::vec3 upVector;


	//glm::vec2 oldCursorPosition, delta;


	Camera(GLfloat sensitivity);
	glm::mat4 getWorldToViewMatrix();

	void updateEyePosition(GLfloat leftOrBack, bool isStrafing);
	void updateViewDirection(glm::vec3 delta);
	void updateUpVector(glm::vec3 delta);
};


#endif // !CAMERA_H


