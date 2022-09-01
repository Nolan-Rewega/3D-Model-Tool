#include "Light.h"

Light::Light( TYPE type,
	          glm::vec3 position,
	          glm::vec3 direction,
	          glm::vec3 ambience,
	          glm::vec3 diffusion,
	          glm::vec3 specularity,
	          glm::vec3 attenuation,
	          glm::vec2 softEdgeConstants )
{
	m_type = type;
	m_shadowMapSize = 1024;

	m_position = position;
	m_direction = direction;

	m_ambient = ambience;
	m_diffuse = diffusion;
	m_specular = specularity;

	m_attenuation = attenuation;
	m_softedge = softEdgeConstants;

	switch (m_type) {
		case TYPE::Directional:
			m_lightModel = new Cube(m_position, m_diffuse, 0.25f);
			generateDepthMap();
			break;
		case TYPE::Point:
			 m_lightModel = new Cube(m_position, m_diffuse, 0.25f);
			//generateCubeMap();
			break;
		case TYPE::Spot:
			m_lightModel = new Tetrahedron(m_position, m_diffuse, 0.25f);
			break;

		default:
			break;
	}

	// -- Orthogonal projections
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 10.0f);
	glm::mat4 lightView = glm::lookAt( m_position,
		                               m_direction,
                                       glm::vec3(0.0f, 1.0f, 0.0f));
	m_lightTransform = lightProjection * lightView;
}

Light::~Light(){
	free(m_lightModel);
}

glm::mat4 Light::getLightTransform(){
	return m_lightTransform;
}

GLuint Light::getShadowMapID(){
	return m_shadowMapID;
}

int Light::getShadowMapSize(){
	return m_shadowMapSize;
}

int Light::getType(){
	return m_type;
}

Shape* Light::getLightModel(){
	return m_lightModel;
}

glm::vec3 Light::getPosition(){
	return m_position;
}

glm::vec3 Light::getDirection(){
	return m_direction;
}

glm::vec3 Light::getAmbience(){
	return m_ambient;
}

glm::vec3 Light::getDiffusion(){
	return m_diffuse;
}

glm::vec3 Light::getSpecularity(){
	return m_specular;
}

glm::vec3 Light::getAttenuationConstants(){
	return m_attenuation;
}

glm::vec2 Light::getSoftEdgeConstants() {
	return m_softedge;
}

// -- Used for Directional and Spot Lights.
void Light::generateDepthMap(){
	//-- Depth map texture initialization.
	glGenTextures(1, &m_shadowMapID);
	glBindTexture(GL_TEXTURE_2D, m_shadowMapID);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_DEPTH_COMPONENT,
		m_shadowMapSize,
		m_shadowMapSize,
		0,
		GL_DEPTH_COMPONENT,
		GL_FLOAT,
		NULL
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}

// -- Used for point lights.
void Light::generateCubeMap(){


}


