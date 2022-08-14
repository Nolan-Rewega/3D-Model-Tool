#ifndef PLANE_H
#define PLANE_H

#include "Shape.h"

class Plane : public Shape {
public:
	Plane(glm::vec3 _position, glm::vec3 _color, GLfloat _width, GLfloat _height);
	~Plane();

private:
	GLfloat width, height;

	void fillVertexData();
	void fillFaces();
};

#endif // !PLANE_H


