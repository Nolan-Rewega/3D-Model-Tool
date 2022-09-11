#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Light.h"

class DirectionalLight: public Light {
public:
	DirectionalLight(
		glm::vec3 direction,
		glm::vec3 ambience,
		glm::vec3 diffusion,
		glm::vec3 specularity
	);
	~DirectionalLight();

	void configureDepthAttachment();
	void renderDepthmap(Shape* shape, GLuint VBO, GLuint VAO);

private:
	void setLightSpaceTransforms();
};

#endif // !DIRECTIONALLIGHT_H

