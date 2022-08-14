#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


class Camera {
public:

	Camera(GLfloat sensitivity, GLfloat givenRadius);
	glm::vec3 getEyePosition();
	glm::vec3 getViewDirection();
	glm::mat4 getWorldToViewMatrix();

	void sphereRotation(GLfloat dTheta, GLfloat dPhi);
	void zoom(GLfloat dZoom);
	void sphereTranslate(glm::vec3 delta);

private:
	int cycle;

	GLfloat SENSE, cameraSpeed;

	GLfloat radius, theta, phi;

	glm::vec3 eyePosition;
	glm::vec3 viewDirection; // -- must be normalized
	glm::vec3 upVector;
	glm::vec3 origin;

	void updateParameters();
};


#endif // !CAMERA_H


