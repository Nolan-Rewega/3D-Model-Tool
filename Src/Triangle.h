#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "Shape.h"


class Triangle : public Shape{
public:
	// -- A Triangle is defined from the bottom vertices and between the top vertices of a square
	Triangle(glm::vec3 pos, glm::vec3 color, GLfloat width, GLfloat height);
	~Triangle();


private:
	GLfloat halfWidth, halfHeight;

	void fillVertexData();
};

#endif // !TRIANGLE_H

