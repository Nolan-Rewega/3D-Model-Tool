#include "Cube.h"


Cube::Cube(glm::vec3 pos, glm::vec3 color, GLfloat width){
	origin = pos;
	shapeColor = color;
	halfWidth = width/2;

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
	GLuint test[36] = 
	{ 
		0,1,2,	1,2,3,	// -- left
		2,3,7,	2,6,7,	// -- back
		7,6,4,	7,5,4,	// -- right
		4,0,1,	4,5,1,	// -- front
		5,3,1,	5,3,7,	// -- top
		4,0,2,	6,4,2	// -- bottom
	};

	for (int i = 0; i < numberOfIndices; i++) {
		indices[i] = test[i];
	}

}




