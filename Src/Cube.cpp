#include "Cube.h"


Cube::Cube(glm::vec3 pos, glm::vec3 color, GLfloat givenWidth){
	origin = pos;
	shapeColor = color;
	width = givenWidth;

	numberOfVertices = 8;
	vertexDataByteSize = numberOfVertices * sizeof(GLfloat) * 6;
	vertexData = (Vertex*)calloc(numberOfVertices, sizeof(Vertex));
	
	numberOfDataIndices = 3 * 2 * 6;
	dataIndicesByteSize = numberOfDataIndices * sizeof(GLushort);
	dataIndices = (GLushort*)calloc(dataIndicesByteSize, sizeof(GLushort));
	
	// -- all triangles are in front facing winding order (CCW).
	GLushort temp[36] =
	{
		2,1,0,	1,2,3,	// -- back
		7,3,2,	2,6,7,	// -- top
		7,6,4,	4,5,7,	// -- front
		4,0,1,	1,5,4,	// -- bottom
		1,3,5,	5,3,7,	// -- right
		2,0,4,	2,4,6	// -- left
	};

	for (unsigned int i = 0; i < numberOfDataIndices; i++) {
		dataIndices[i] = temp[i];
	}

	numberOfOutlineIndices = 4*3*2;
	outlineIndiceByteSize = numberOfOutlineIndices * sizeof(GLushort);
	outlineIndices = (GLushort*)calloc(outlineIndiceByteSize, sizeof(GLushort));

	// -- outline
	GLushort tempOutline [24] =
	{
		0,1, 0,4, 0,2, 1,3, 1,5, 2,3,
		2,6, 3,7, 4,5, 4,6, 5,7, 6,7
	};

	for (unsigned int i = 0; i < numberOfDataIndices; i++) {
		outlineIndices[i] = tempOutline[i];
	}


	translationMatrix = glm::mat4(1.0f);
	rotationMatrix = glm::mat4(1.0f);

	fillVertexData();
}
Cube::~Cube() {
	freeVertexData();
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
}




