#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <iostream>

class Light {
public:
	glm::vec3 getAmbience();
	glm::vec3 getDiffustion();
	glm::vec3 getSpecularity();

protected:
	// -- private constructor.
	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

	// -- Light intensity values.
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
};


#endif // !LIGHT_H



