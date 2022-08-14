#include "Light.h"

glm::vec3 Light::getAmbience(){
	return m_ambient;
}

glm::vec3 Light::getDiffustion(){
	return m_diffuse;
}

glm::vec3 Light::getSpecularity(){
	return m_specular;
}

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular){
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;
	std::cout << "IN LIGHT CONSTUCTOR\n";
}
