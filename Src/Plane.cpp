#include "Plane.h"

Plane::Plane(glm::vec3 pos, glm::vec3 color, GLfloat givenWidth, GLfloat givenHeight){
	origin = pos;
	shapeColor = color;
	width = givenWidth;
	height = givenHeight;

	numberOfVertices = 4;
	vertexDataByteSize = numberOfVertices * sizeof(GLfloat) * 6;
	vertexData = (Vertex*)calloc(numberOfVertices, sizeof(Vertex));

	numberOfDataIndices = 6;
	dataIndicesByteSize = numberOfDataIndices * sizeof(GLushort);
	dataIndices = (GLushort*)calloc(numberOfDataIndices, sizeof(GLushort));

	translationMatrix = glm::mat4(1.0f);
	rotationMatrix = glm::mat4(1.0f);

	fillVertexData();
}

Plane::~Plane(){
	freeVertexData();
}



void Plane::fillVertexData(){
	GLfloat halfWidth = width / 2;
	GLfloat halfHeight= height / 2;
	vertexData[0] = { glm::vec3(origin.x + halfWidth, 0.0f, origin.z - halfHeight), shapeColor };
	vertexData[1] = { glm::vec3(origin.x + halfWidth, 0.0f, origin.z + halfHeight), shapeColor };
	vertexData[2] = { glm::vec3(origin.x - halfWidth, 0.0f, origin.z - halfHeight), shapeColor };
	vertexData[3] = { glm::vec3(origin.x - halfWidth, 0.0f, origin.z + halfHeight), shapeColor };

	dataIndices[0] = 0; dataIndices[1] = 2; dataIndices[2] = 1;
	dataIndices[3] = 3; dataIndices[4] = 1; dataIndices[5] = 2;
}
