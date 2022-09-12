#ifndef LIGHT_H
#define LIGHT_H

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Primitives/Cube.h"
#include "../Primitives/Tetrahedron.h"
#include "../OpenGL/ShaderProgram.h"

class Light {
public:

	std::vector<glm::mat4> getLightTransforms();

	GLuint getShadowMapID();
	int getShadowMapSize();

	int getType();
	Shape* getLightModel();

	float getFarPlane();

	glm::vec3 getPosition();
	glm::vec3 getDirection();

	glm::vec3 getAmbience();
	glm::vec3 getDiffusion();
	glm::vec3 getSpecularity();

	glm::vec3 getAttenuationConstants();
	glm::vec2 getSoftEdgeConstants();

	void setPosition(glm::vec3 delta);

	virtual void configureDepthAttachment() = 0;
	virtual void renderDepthmap(Shape* shape, GLuint VBO, GLuint VAO) = 0;

protected:
	// -- Shadow program.
	ShaderProgram* m_shadowProgram;

	// -- Light Matricies
	std::vector<glm::mat4> m_lightTransforms;
	
	// -- Type of light.
	int m_type;

	float m_farPlane;
	
	// -- Light model
	Shape* m_lightModel;

	// -- Light shadow map ID
	GLuint m_shadowMapID;
	GLuint m_shadowMapSize;

	// -- Light vector values.
	glm::vec3 m_position;
	glm::vec3 m_direction;

	// -- Light intensity values.
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;

	// -- Light modification values.
	glm::vec3 m_attenuation;
	glm::vec2 m_softedge;


	virtual void setLightSpaceTransforms() = 0;

};


#endif // !LIGHT_H



