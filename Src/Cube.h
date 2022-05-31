#ifndef CUBE_H
#define CUBE_H
#include "Shape.h"

class Cube : public Shape{
public:
	Cube(glm::vec3 pos, glm::vec3 color, GLfloat width);
	~Cube();

private:
	GLfloat width;
	void fillVertexData();
};


#endif // !CUBE_H
