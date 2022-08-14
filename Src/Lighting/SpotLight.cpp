#include "SpotLight.h"

SpotLight::SpotLight( glm::vec3 position,
	                  glm::vec3 direction,
	                  glm::vec3 diffusion,
	                  glm::vec3 specular,
	                  glm::vec2 attenuationConstants,
	                  glm::vec2 softnessConstants
                    ) : PointLight( position,
						            glm::vec3(0.0f, 0.0f, 0.0f),
						            diffusion,
						            specular,
						            attenuationConstants )
{
	m_direction = direction;

	m_phi   = softnessConstants.x;
	m_gamma = softnessConstants.y;

	std::cout << "IN SPOTLIGHT CONSTUCTOR\n";
}

glm::vec2 SpotLight::getSoftnessConstants(){
	return glm::vec2(m_phi, m_gamma);
}
