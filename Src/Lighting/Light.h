#ifndef LIGHT_H
#define LIGHT_H

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Primitives/Cube.h"
#include "../Primitives/Tetrahedron.h"


class Light {
public:
	enum TYPE { Directional, Point, Spot};

	// -- public constructor.
	Light(
		TYPE type,
		glm::vec3 position,
		glm::vec3 direction,
		glm::vec3 ambience,
		glm::vec3 diffusion,
		glm::vec3 specularity,
		glm::vec3 attenuation,
		glm::vec2 softEdgeConstants
	);
	~Light();

	glm::mat4 getLightTransform();

	GLuint getShadowMapID();
	int getShadowMapSize();

	int getType();
	Shape* getLightModel();

	glm::vec3 getPosition();
	glm::vec3 getDirection();

	glm::vec3 getAmbience();
	glm::vec3 getDiffusion();
	glm::vec3 getSpecularity();

	glm::vec3 getAttenuationConstants();
	glm::vec2 getSoftEdgeConstants();

private:
	// -- Light Type.
	TYPE m_type;

	// -- Light Matricies
	glm::mat4 m_lightTransform;

	// -- Light shadow map ID
	GLuint m_shadowMapID;
	GLuint m_shadowMapSize;

	// -- Light model
	Shape* m_lightModel;

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

	// -- Shadow map generation methods.
	void generateDepthMap();
	void generateCubeMap();

};


#endif // !LIGHT_H



