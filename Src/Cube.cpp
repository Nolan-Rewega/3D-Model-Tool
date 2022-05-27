#include "Cube.h"


Cube::Cube(glm::vec3 pos, glm::vec3 color, GLfloat givenWidth){
	origin = pos;
	shapeColor = color;
	width = givenWidth;

	numberOfVertices = 8;
	vertexDataByteSize = numberOfVertices * sizeof(GLfloat) * 6;
	vertexData = (Vertex*)calloc(numberOfVertices, sizeof(Vertex));
	
	numberOfIndices = 3 * 2 * 6;
	indicesByteSize = numberOfIndices * sizeof(GLushort);
	indices = (GLushort*)calloc(numberOfIndices, sizeof(GLushort));
	
	translationMatrix = glm::mat4(1.0f);
	rotationMatrix = glm::mat4(1.0f);

	fillVertexData();
}
Cube::~Cube() {
	freeVertexData();
}




void Cube::translateShape(glm::vec3 targetPos){
	translationMatrix = glm::translate(glm::mat4(1.0f), targetPos);
	// fillVertexData();
}

void Cube::rotateShape(glm::vec3 angleVec, GLfloat angle) {
	// -- remember, move the shape to the origin then rotate.
	rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), angleVec);
}

void Cube::fillVertexData() {
	GLfloat halfWidth = width / 2;
	GLfloat x[2] = { origin.x - halfWidth, origin.x + halfWidth };
	GLfloat y[2] = { origin.y - halfWidth, origin.y + halfWidth };
	GLfloat z[2] = { origin.z - halfWidth, origin.z + halfWidth };

	// 0 idx = left of origin,  1 idx = right of origin.
	for (int xIdx = 0; xIdx <= 1; xIdx++) {
		for (int yIdx = 0; yIdx <= 1; yIdx++) {
			for (int zIdx = 0; zIdx <= 1; zIdx++) {
				int idx1D = xIdx + 2 * (yIdx + 2 * zIdx);
				vertexData[idx1D] = { glm::vec3(x[xIdx], y[yIdx], z[zIdx]), glm::vec3((x[xIdx] + 1) / 2, (y[yIdx] + 1) / 2, (z[zIdx] + 1) / 2 )};
			}
		}
	}

	// -- update triangulation indicies
	// -- all triangles are in front facing winding order (CCW).
	GLuint test[36] = 
	{ 
		2,1,0,	1,2,3,	// -- back
		7,3,2,	2,6,7,	// -- top
		7,6,4,	4,5,7,	// -- front
		4,0,1,	1,5,4,	// -- bottom
		1,3,5,	5,3,7,	// -- right
		2,0,4,	2,4,6	// -- left
	};

	for (int i = 0; i < numberOfIndices; i++) {
		indices[i] = test[i];
	}

}




