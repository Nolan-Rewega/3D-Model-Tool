#include "DirectionalLight.h"

DirectionalLight::DirectionalLight( glm::vec3 direction,
	                                glm::vec3 ambience,
	                                glm::vec3 diffusion,
	                                glm::vec3 specular
                                  ) : Light(ambience, diffusion, specular)
{
	m_direction = direction;
}
