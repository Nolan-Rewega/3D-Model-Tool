#include "Icosahedron.h"

Icosahedron::Icosahedron(glm::vec3 pos, glm::vec3 color, GLfloat givenWidth){
	origin = pos;
	shapeColor = color;
	width = givenWidth;

	numberOfVertices = 12;
	vertexDataByteSize = numberOfVertices * sizeof(GLfloat) * 6;
	vertexData = (Vertex*)calloc(numberOfVertices, sizeof(Vertex));

	numberOfIndices = 20 * 3;
	indicesByteSize = numberOfIndices * sizeof(GLushort);
	indices = (GLushort*)calloc(numberOfIndices, sizeof(GLushort));

	translationMatrix = glm::mat4(1.0f);
	rotationMatrix = glm::mat4(1.0f);

	fillVertexData();
}

Icosahedron::~Icosahedron(){
	freeVertexData();
}

void Icosahedron::translateShape(glm::vec3 targetPos){
	translationMatrix = glm::translate(glm::mat4(1.0f), targetPos);
}

void Icosahedron::rotateShape(glm::vec3 angleVec, GLfloat angle){
	rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), angleVec);
}

void Icosahedron::fillVertexData() {
	GLfloat halfWidth = width / 2;
	GLfloat quarterWidth = halfWidth / 2;

	// -- plane one
	vertexData[0] = { glm::vec3(origin.x - quarterWidth, 0.0f, origin.z + halfWidth), glm::vec3(0.0f,1.0f,0.0f) };
	vertexData[1] = { glm::vec3(origin.x + quarterWidth, 0.0f, origin.z + halfWidth), glm::vec3(0.0f,1.0f,0.0f) };
	vertexData[2] = { glm::vec3(origin.x - quarterWidth, 0.0f, origin.z - halfWidth), glm::vec3(0.0f,1.0f,0.0f) };
	vertexData[3] = { glm::vec3(origin.x + quarterWidth, 0.0f, origin.z - halfWidth), glm::vec3(0.0f,1.0f,0.0f) };

	// -- plane two
	vertexData[4] = { glm::vec3(origin.x + halfWidth, origin.y + quarterWidth, 0.0f), glm::vec3(0.0f,1.0f,0.0f) };
	vertexData[5] = { glm::vec3(origin.x + halfWidth, origin.y - quarterWidth, 0.0f), glm::vec3(0.0f,1.0f,0.0f) };
	vertexData[6] = { glm::vec3(origin.x - halfWidth, origin.y + quarterWidth, 0.0f), glm::vec3(0.0f,1.0f,0.0f) };
	vertexData[7] = { glm::vec3(origin.x - halfWidth, origin.y - quarterWidth, 0.0f), glm::vec3(0.0f,1.0f,0.0f) };

	// -- plane three
	vertexData[8]  = { glm::vec3(0.0f, origin.y + halfWidth, origin.z + quarterWidth), glm::vec3(0.0f,1.0f,0.0f) };
	vertexData[9]  = { glm::vec3(0.0f, origin.y + halfWidth, origin.z - quarterWidth), glm::vec3(0.0f,1.0f,0.0f) };
	vertexData[10] = { glm::vec3(0.0f, origin.y - halfWidth, origin.z + quarterWidth), glm::vec3(0.0f,1.0f,0.0f) };
	vertexData[11] = { glm::vec3(0.0f, origin.y - halfWidth, origin.z - quarterWidth), glm::vec3(0.0f,1.0f,0.0f) };
	
	
	GLuint test[60] =
	{	
		8,  0,  1,		8,  1,  4,		6,  0,  8,		0, 10,  1,
		1, 10,  5,		1,  5,  4,		4,  5,  3,		8,  4,  9,
		6,  8,  9,		9,  4,  3,		9,  2,  6,		9,  3,  2,
		3, 11,  2,		3,  5, 11,		2, 11,  7,		5, 10, 11,
		7, 11, 10,		0,  7, 10,		6,  2,  7,		6,  7,  0,
	};

	for (int i = 0; i < numberOfIndices; i++) {
		indices[i] = test[i];
	}
}
