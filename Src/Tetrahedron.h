#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#include "Shape.h"
class Tetrahedron : public Shape {
public:
	Tetrahedron(glm::vec3 _position, glm::vec3 _color, GLfloat _width);
	~Tetrahedron();

private:
	GLfloat width;
	void fillVertexData();
	void fillFaces();
};

#endif // !TETRAHEDRON_H




