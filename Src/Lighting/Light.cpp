#include "Light.h"


// -- Getters
std::vector<glm::mat4> Light::getLightTransforms(){
	return m_lightTransforms;
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

// -- Setters
void Light::setPosition(glm::vec3 delta) {
	m_position += delta;
	this->setLightSpaceTransforms();
}