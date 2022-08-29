#include "Tetrahedron.h"

Tetrahedron::Tetrahedron(glm::vec3 _position, glm::vec3 _color, GLfloat _width) {
	origin = _position;
	color = _color;
	width = _width;

	// -- Set Geometry values.
	numberOfVertices = 4;
	numberOfFaces = 4;
	initialization();

	// -- Initial Matrice definitions.
	translationMatrix = glm::mat4(1.0f);
	rotationMatrix = glm::mat4(1.0f);

	fillVertexData();
	fillFaces();
	calculateSurfaceNormals();
	convertToBuffer();
}

Tetrahedron::~Tetrahedron(){
	freeArrays();
}


void Tetrahedron::fillFaces() {
	// -- all triangles are in front facing winding order (CCW).
	GLushort indices[12] = { 0,2,1,  3,0,1,  3,1,2,  3,2,0, };

	for (unsigned int i = 0; i < numberOfFaces * 3; i++) {
		faces[i] = indices[i];
	}

}

void Tetrahedron::fillVertexData(){
	GLfloat halfWidth = width / 2;
	GLfloat hyp = halfWidth / (glm::sin(glm::radians(60.0f)));
	GLfloat adj = hyp * glm::cos(glm::radians(60.0f));

	GLfloat len = width * glm::sin(glm::radians(60.0f));
	GLfloat angle = glm::asin(2.0f / 3.0f * len);
	GLfloat height = glm::cos(angle) * width;

	vertexData[0] =
	{ 
		glm::vec3(origin.x - halfWidth, origin.y, origin.z + adj),
		color
	};

	vertexData[1] =
	{
		glm::vec3(origin.x + halfWidth, origin.y, origin.z + adj),
		color
	};

	vertexData[2] = 
	{ 
		glm::vec3(origin.x, origin.y, origin.z - hyp),
		color
	};
	vertexData[3] = 
	{ 
		glm::vec3(origin.x, origin.y + height, origin.z),
		color
	};

}
