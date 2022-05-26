#ifndef SHAPE_H
#define SHAPE_H
#include <iostream>
#include <vector>

#include "Vertex.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>


class Shape {
public:
	virtual void translateShape(glm::vec3 targetPos) = 0;
	virtual void rotateShape(glm::vec3 angleVec, GLfloat angle) = 0;
	
	Vertex* getVertexData();
	GLuint getVertexDataSizeInBytes();
	GLushort* getIndices();
	GLuint getIndicesSizeInBytes();
	
	glm::mat4 getRotationMatrix();
	glm::mat4 getTranslationMatrix();

protected:
	Vertex* vertexData;
	GLuint numberOfVertices;
	GLuint vertexDataByteSize;

	GLushort* indices;
	GLuint numberOfIndices;
	GLuint indicesByteSize;

	glm::vec3 origin;
	glm::vec3 shapeColor;
	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;


	void freeVertexData();

};




#endif // !SHAPE_H

