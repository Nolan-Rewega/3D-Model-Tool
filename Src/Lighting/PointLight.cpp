#include "PointLight.h"

PointLight::PointLight( glm::vec3 position,
	                    glm::vec3 ambience,
	                    glm::vec3 diffusion,
	                    glm::vec3 specular,
	                    glm::vec2 attenuationConstants
                      ) : Light(ambience, diffusion, specular)
{	
	m_position = position;

	m_constantTerm = 1.0f;
	m_linearTerm = attenuationConstants.x;
	m_quadraticTerm = attenuationConstants.y;
	std::cout << "IN POINTLIGHT CON\n";
}

glm::vec3 PointLight::getAttenuationConstants(){
	return glm::vec3(1.0f, m_linearTerm, m_quadraticTerm);
}
