#include "Triangle.h"

Triangle::Triangle(glm::vec3 pos, glm::vec3 color, GLfloat width, GLfloat height){
	origin = pos;
	shapeColor = color;
	halfWidth = width / 2;
	halfHeight = height / 2;

	numberOfVertices = 3;
	vertexDataByteSize = numberOfVertices * sizeof(GLfloat) * 6;
	vertexData = (Vertex*)calloc(numberOfVertices, sizeof(Vertex));

	numberOfIndices = 3;
	indicesByteSize = numberOfIndices * sizeof(GLushort);
	indices = (GLushort*)calloc(numberOfIndices, sizeof(GLushort));

	translationMatrix = glm::mat4(1.0f);
	rotationMatrix = glm::mat4(1.0f);

	fillVertexData();
}

Triangle::~Triangle(){
	freeVertexData();
}

void Triangle::translateShape(glm::vec3 targetPos) {
	translationMatrix = glm::translate(glm::mat4(1.0f), targetPos);
	// fillVertexData();
}

void Triangle::rotateShape(glm::vec3 angleVec, GLfloat angle) {
	// -- remember, move the shape to the origin then rotate.
	rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), angleVec);
}



void Triangle::fillVertexData(){
	vertexData[0] = { glm::vec3(origin.x - halfWidth, origin.y - halfWidth, 0.0f), shapeColor };
	vertexData[1] = { glm::vec3(origin.x + halfWidth, origin.y - halfWidth, 0.0f), shapeColor };
	vertexData[2] = { glm::vec3(origin.x, origin.y + halfWidth, 0.0f), shapeColor };

	indices[0] = 2; indices[1] = 0; indices[2] = 1;

}
