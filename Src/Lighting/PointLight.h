#ifndef PLIGHT_H
#define PLIGHT_H

#include "Light.h"

class PointLight : public Light {
public:
	PointLight(
		glm::vec3 position,
		glm::vec3 ambience,
		glm::vec3 diffusion,
		glm::vec3 specular,
		glm::vec2 attenuationConstants
	);

	glm::vec3 getAttenuationConstants();

protected:
	glm::vec3 m_position;

	// -- Light attenuation constants
	float m_constantTerm;   // -- Fixed to 1.0f
	float m_linearTerm;     // -- m_linearTerm > 0
	float m_quadraticTerm;  // -- m_quadraticTerm > 0

};


#endif // !PLIGHT_H
