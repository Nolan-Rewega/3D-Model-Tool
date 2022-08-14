#include "Plane.h"

Plane::Plane(glm::vec3 _position, glm::vec3 _color, GLfloat _width, GLfloat _height) {
	origin = _position;
	color  = _color;
	width  = _width;
	height = _height;

	// -- Set Geometry values.
	numberOfVertices = 4;
	numberOfFaces = 2;
	initialization();

	// -- Initial Matrice definitions.
	translationMatrix = glm::mat4(1.0f);
	rotationMatrix = glm::mat4(1.0f);

	fillVertexData();
	fillFaces();
	calculateSurfaceNormals();
	convertToBuffer();
}

Plane::~Plane(){
	freeArrays();
}

void Plane::fillFaces() {
	faces[0] = 0; faces[1] = 2; faces[2] = 1;
	faces[3] = 3; faces[4] = 1; faces[5] = 2;
}


void Plane::fillVertexData(){
	GLfloat halfWidth  = width / 2;
	GLfloat halfHeight = height / 2;
	vertexData[0] = { glm::vec3(origin.x + halfWidth, 0.0f, origin.z - halfHeight), color };
	vertexData[1] = { glm::vec3(origin.x + halfWidth, 0.0f, origin.z + halfHeight), color };
	vertexData[2] = { glm::vec3(origin.x - halfWidth, 0.0f, origin.z - halfHeight), color };
	vertexData[3] = { glm::vec3(origin.x - halfWidth, 0.0f, origin.z + halfHeight), color };
}
