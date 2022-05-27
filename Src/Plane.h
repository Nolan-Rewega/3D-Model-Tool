#ifndef PLANE_H
#define PLANE_H

#include "Shape.h"

class Plane : public Shape {
public:
	Plane(glm::vec3 pos, glm::vec3 color, GLfloat width, GLfloat height);
	~Plane();

private:
	GLfloat width, height;

	void fillVertexData();
};

#endif // !PLANE_H


