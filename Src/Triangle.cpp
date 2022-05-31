#include "Triangle.h"

Triangle::Triangle(glm::vec3 pos, glm::vec3 color, GLfloat width, GLfloat height){
	origin = pos;
	shapeColor = color;
	halfWidth = width / 2;
	halfHeight = height / 2;

	numberOfVertices = 3;
	vertexDataByteSize = numberOfVertices * sizeof(GLfloat) * 6;
	vertexData = (Vertex*)calloc(numberOfVertices, sizeof(Vertex));

	numberOfDataIndices = 3;
	dataIndicesByteSize = numberOfDataIndices * sizeof(GLushort);
	dataIndices = (GLushort*)calloc(numberOfDataIndices, sizeof(GLushort));

	translationMatrix = glm::mat4(1.0f);
	rotationMatrix = glm::mat4(1.0f);

	fillVertexData();
}

Triangle::~Triangle(){
	freeVertexData();
}



void Triangle::fillVertexData(){
	vertexData[0] = { glm::vec3(origin.x - halfWidth, origin.y - halfWidth, 0.0f), shapeColor };
	vertexData[1] = { glm::vec3(origin.x + halfWidth, origin.y - halfWidth, 0.0f), shapeColor };
	vertexData[2] = { glm::vec3(origin.x, origin.y + halfWidth, 0.0f), shapeColor };

	dataIndices[0] = 2; dataIndices[1] = 0; dataIndices[2] = 1;

}
