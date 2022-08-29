#include "Icosahedron.h"

Icosahedron::Icosahedron(glm::vec3 _position, glm::vec3 _color, GLfloat _width){
	origin = _position;
	color = _color;
	width = _width;

	// -- Set Geometry values.
	numberOfVertices = 12;
	numberOfFaces = 20;
	initialization();

	// -- Initial Matrice definitions.
	translationMatrix = glm::mat4(1.0f);
	rotationMatrix = glm::mat4(1.0f);

	fillVertexData();
	fillFaces();
	calculateSurfaceNormals();
	convertToBuffer();
}

Icosahedron::~Icosahedron(){
	freeArrays();
}


void Icosahedron::classOneRecursiveHelper( glm::vec3 v1,
										   glm::vec3 v2,
										   glm::vec3 v3,
										   int depth,
										   Vertex* VDC,
										   GLushort* FDC){
	if (depth == 0) {
		// -- Create three new Vertices, and one new Face.
		// TODO:
	}

	// -- calculate the 3 new vertices.
	glm::vec3 v12( (v1 + v2) /= 2.0 );
	glm::vec3 v13( (v1 + v3) /= 2.0 );
	glm::vec3 v23( (v2 + v3) /= 2.0 );

	// -- normalized them.
	v12 /= v12.length();
	v13 /= v13.length();
	v23 /= v23.length();

	// -- subdived further.
	classOneRecursiveHelper( v1, v12, v13, depth - 1, VDC, FDC);
	classOneRecursiveHelper(v12,  v2, v23, depth - 1, VDC, FDC);
	classOneRecursiveHelper(v12, v23, v13, depth - 1, VDC, FDC);
	classOneRecursiveHelper(v13, v23,  v3, depth - 1, VDC, FDC);
}

void Icosahedron::classOneSubdivision(int depth){

	// -- Create a vertexData and Face copys to add new faces and verts to.
	Vertex* copyVD = (Vertex*)calloc(numberOfFaces * 3 * 9, sizeof(Vertex));
	GLushort* copyFD = (GLushort*)calloc(numberOfFaces * 3 * 9, sizeof(GLushort));

	// -- Sub-Divide each face.
	for (int i = 0; i < numberOfFaces * 3; i += 3) {
		// -- Get position information.
		glm::vec3 v1 = vertexData[faces[i + 0]].position;
		glm::vec3 v2 = vertexData[faces[i + 1]].position;
		glm::vec3 v3 = vertexData[faces[i + 2]].position;

		// -- Call the helper function.
		classOneRecursiveHelper(v1, v2, v3, depth, copyVD, copyFD);
	}

	numberOfFaces *= 4;
	
	free(vertexData);
	free(faces);

	// -- Swap the pointers. (I think this works)
	vertexData = copyVD;
	faces = copyFD;

	// -- Update normals and fill the OpenGL buffer. 
	calculateSurfaceNormals();
	convertToBuffer();
}


void Icosahedron::fillFaces() {
	GLushort indices[] =
	{
		8,  0,  1,		8,  1,  4,		6,  0,  8,		0, 10,  1,
		1, 10,  5,		1,  5,  4,		4,  5,  3,		8,  4,  9,
		6,  8,  9,		9,  4,  3,		9,  2,  6,		9,  3,  2,
		3, 11,  2,		3,  5, 11,		2, 11,  7,		5, 10, 11,
		7, 11, 10,		0,  7, 10,		6,  2,  7,		6,  7,  0,
	};

	for (int i = 0; i < numberOfFaces * 3; ++i) {
		faces[i] = indices[i];
	}
}

void Icosahedron::fillVertexData() {
	GLfloat halfWidth = width / 2;
	GLfloat quarterWidth = halfWidth / 2;

	// -- plane one
	vertexData[0] = { glm::vec3(origin.x - quarterWidth, 0.0f, origin.z + halfWidth), color };
	vertexData[1] = { glm::vec3(origin.x + quarterWidth, 0.0f, origin.z + halfWidth), color };
	vertexData[2] = { glm::vec3(origin.x - quarterWidth, 0.0f, origin.z - halfWidth), color };
	vertexData[3] = { glm::vec3(origin.x + quarterWidth, 0.0f, origin.z - halfWidth), color };

	// -- plane two
	vertexData[4] = { glm::vec3(origin.x + halfWidth, origin.y + quarterWidth, 0.0f), color };
	vertexData[5] = { glm::vec3(origin.x + halfWidth, origin.y - quarterWidth, 0.0f), color };
	vertexData[6] = { glm::vec3(origin.x - halfWidth, origin.y + quarterWidth, 0.0f), color };
	vertexData[7] = { glm::vec3(origin.x - halfWidth, origin.y - quarterWidth, 0.0f), color };

	// -- plane three
	vertexData[8]  = { glm::vec3(0.0f, origin.y + halfWidth, origin.z + quarterWidth), color };
	vertexData[9]  = { glm::vec3(0.0f, origin.y + halfWidth, origin.z - quarterWidth), color };
	vertexData[10] = { glm::vec3(0.0f, origin.y - halfWidth, origin.z + quarterWidth), color };
	vertexData[11] = { glm::vec3(0.0f, origin.y - halfWidth, origin.z - quarterWidth), color };
}
