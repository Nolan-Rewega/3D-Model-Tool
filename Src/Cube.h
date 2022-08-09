#ifndef CUBE_H
#define CUBE_H
#include "Shape.h"

class Cube : public Shape{
public:
	Cube(glm::vec3 _position, glm::vec3 _color, GLfloat _width);
	~Cube();

private:
	GLfloat width;

	void fillVertexData();
	void fillFaces();
};


#endif // !CUBE_H
