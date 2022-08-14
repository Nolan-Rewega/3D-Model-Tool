#ifndef DLIGHT_H
#define DLIGHT_H

#include "Light.h"

class DirectionalLight : public Light{
public:
	DirectionalLight(
		glm::vec3 direction,
		glm::vec3 ambience,
		glm::vec3 diffusion,
		glm::vec3 specular
	);

private:
	glm::vec3 m_direction;

};

#endif // !DLIGHT_H


