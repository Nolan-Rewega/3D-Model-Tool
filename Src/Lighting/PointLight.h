#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Light.h"


class PointLight : public Light{
public:
	PointLight(
		glm::vec3 position,
		glm::vec3 ambience,
		glm::vec3 diffusion,
		glm::vec3 specularity,
		glm::vec3 attenuation,
		glm::vec2 softEdgeConstants
	);
	~PointLight();

	void configureDepthAttachment();
	void renderDepthmap(Shape* shape, GLuint VBO, GLuint VAO);

private:
	void setLightSpaceTransforms();
};


#endif //!POINTLIGHT_H
