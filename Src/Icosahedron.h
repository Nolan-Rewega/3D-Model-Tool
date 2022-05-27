#ifndef ICOSAHEDRON_H
#define ICOSAHEDRON_H
#include "Shape.h"

class Icosahedron : public Shape{
public:
	Icosahedron(glm::vec3 pos, glm::vec3 color, GLfloat width);
	~Icosahedron();

	void translateShape(glm::vec3 targetPos);
	void rotateShape(glm::vec3 angleVec, GLfloat angle);

private:
	GLfloat width;

	void fillVertexData();
};



#endif // !ICOSAHEDRON_H



