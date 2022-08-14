#ifndef ICOSAHEDRON_H
#define ICOSAHEDRON_H
#include "Shape.h"

class Icosahedron : public Shape{
public:
	Icosahedron(glm::vec3 _position, glm::vec3 _color, GLfloat _width);
	~Icosahedron();

	void classOneSubdivision(int depth);

private:
	GLfloat width;

	void classOneRecursiveHelper(
		glm::vec3 v1,
		glm::vec3 v2,
		glm::vec3 v3,
		int depth,
		Vertex* VDC,
		GLushort* FDC
	);

	void fillVertexData();
	void fillFaces();
};



#endif // !ICOSAHEDRON_H



