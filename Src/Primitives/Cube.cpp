#include "Cube.h"


Cube::Cube(glm::vec3 _position, glm::vec3 _color, GLfloat _width){
	// -- Set Cube parameters
	origin = _position;
	color = _color;
	width = _width;

	// -- Set Geometry values.
	numberOfVertices = 8;
	numberOfFaces = 12;
	initialization();

	// -- Initial Matrice definitions.
	translationMatrix = glm::mat4(1.0f);
	rotationMatrix = glm::mat4(1.0f);

	fillVertexData();
	fillFaces();
	calculateSurfaceNormals();
	convertToBuffer();
}
Cube::~Cube() {
	freeArrays();
}


void Cube::fillFaces(){
	GLushort indices[] = {
		2,1,0,	1,2,3,	7,3,2,	2,6,7,	7,6,4,	4,5,7,
		4,0,1,	1,5,4,	1,3,5,	5,3,7,	2,0,4,	2,4,6
	};

	for (int i = 0; i < numberOfFaces * 3; ++i) {
		faces[i] = indices[i];
	}
}

void Cube::fillVertexData() {
	GLfloat halfWidth = width / 2;
	GLfloat x[2] = { origin.x - halfWidth, origin.x + halfWidth };
	GLfloat y[2] = { origin.y - halfWidth, origin.y + halfWidth };
	GLfloat z[2] = { origin.z - halfWidth, origin.z + halfWidth };

	for (int xIdx = 0; xIdx <= 1; xIdx++) {
		for (int yIdx = 0; yIdx <= 1; yIdx++) {
			for (int zIdx = 0; zIdx <= 1; zIdx++) {
				int idx1D = xIdx + 2 * (yIdx + 2 * zIdx);
				vertexData[idx1D] = { glm::vec3(x[xIdx], y[yIdx], z[zIdx]),color };
			}
		}
	}
}
