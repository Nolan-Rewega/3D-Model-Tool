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
	void translateShape(glm::vec3 targetPos);
	void rotateShape(glm::vec3 angleVec, GLfloat angle);
	
	GLfloat* getShapeBuffer();
	GLuint getShapeBufferSizeInBytes();
	GLuint getNumberOfBufferVertices();

	glm::mat4 getRotationMatrix();
	glm::mat4 getTranslationMatrix();


	GLfloat* debugConvertToLineNormals();

protected:
	// -- Position      (x,y,z)
	// -- Color         (r,g,b)
	Vertex* vertexData;
	GLuint vertexDataByteSize;
	GLuint numberOfVertices;

	// -- Normals       (u,v,w)
	glm::vec3* normals;
	GLuint normalsByteSize;

	// -- Faces, represented by vertexData indices.
	GLushort* faces;
	GLuint facesByteSize;
	GLuint numberOfFaces;

	// -- openGL buffer
	GLfloat* buffer;
	GLuint bufferByteSize;

	// -- Common shape parameters.
	glm::vec3 origin;
	glm::vec3 color;
	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;

	virtual void fillVertexData() = 0;
	virtual void fillFaces() = 0;

	void initialization();

	void convertToBuffer();
	void calculateSurfaceNormals();

	void freeArrays();
};




#endif // !SHAPE_H

