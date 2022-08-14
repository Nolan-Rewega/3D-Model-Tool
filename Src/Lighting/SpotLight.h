#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Light.h"
#include "PointLight.h"

class SpotLight : public PointLight{
	// -- Spotlight is not really a point light,
	//    I wrote the  inheritance like  this to 
	//    avoid  duplicate code. In reality, there
	//    should be a softedge and an attenuation
	//    class.
public:
	SpotLight(
		glm::vec3 position,
		glm::vec3 direction,
		glm::vec3 diffusion,
		glm::vec3 specular,
		glm::vec2 attenuationConstants,
		glm::vec2 softnessConstants
	);

	glm::vec2 getSoftnessConstants();

private:

	glm::vec3 m_direction;

	// -- Softness constants
	float m_phi;   // -- The angle of the outer circle in degrees
	float m_gamma; // -- The angle of the inner circle in degrees
};

#endif // !SPOTLIGHT_H

