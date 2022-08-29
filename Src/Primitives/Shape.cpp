#include "Shape.h"

GLfloat* Shape::getShapeBuffer(){ return buffer; }
GLuint Shape::getShapeBufferSizeInBytes(){ return bufferByteSize; }
GLuint Shape::getNumberOfBufferVertices(){ return numberOfFaces * 3; }


glm::mat4 Shape::getRotationMatrix(){ return rotationMatrix; }
glm::mat4 Shape::getTranslationMatrix(){return translationMatrix; }

void Shape::convertToBuffer(){
    int n = 0;
    int idx = 0;

    // -- Fill the buffer vertex by vertex.
    for (int i = 0; i < numberOfFaces * 3; ++i) {
        buffer[idx + 0] = vertexData[faces[i]].position.x;
        buffer[idx + 1] = vertexData[faces[i]].position.y;
        buffer[idx + 2] = vertexData[faces[i]].position.z;
        buffer[idx + 3] = color.r;
        buffer[idx + 4] = color.g;
        buffer[idx + 5] = color.b;
        buffer[idx + 6] = normals[n].x;
        buffer[idx + 7] = normals[n].y;
        buffer[idx + 8] = normals[n].z;

        n += ((i + 1) % 3 == 0) ? 1 : 0;
        idx += 9;
    }
}


void Shape::calculateSurfaceNormals() {
    // -- Fill in normals array.
    for (int k = 0; k < numberOfFaces * 3; k += 3) {
        glm::vec3 v1 = vertexData[faces[k + 0]].position;
        glm::vec3 v2 = vertexData[faces[k + 1]].position;
        glm::vec3 v3 = vertexData[faces[k + 2]].position;
        
        // -- edges:   U = v2 - v1,  V = v3 - v1
        glm::vec3 U = v2 - v1;
        glm::vec3 V = v3 - v1;

        glm::vec3 surfaceNormal = glm::cross(U, V);
        // -- Manual cross product calculation.
        // surfaceNormal.x = (U.y * V.z) - (U.z * V.y);
        // surfaceNormal.y = (U.z * V.x) - (U.x * V.z);
        // surfaceNormal.z = (U.x * V.y) - (U.y * V.x);

        normals[k / 3] = surfaceNormal;
    }
}

void Shape::translateShape(glm::vec3 targetPos) {
    translationMatrix = glm::translate(glm::mat4(1.0f), targetPos);
}

void Shape::rotateShape(glm::vec3 angleVec, GLfloat angle) {
    rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), angleVec);
}

void Shape::initialization() {
    /* NOTE MUST HAVE numberOfVertices AND numberOfFaces DEFINED BEFORE CALL */
    if (numberOfVertices == NULL || numberOfFaces == NULL) {
        std::cout << "ERROR IN Shape::initialization(),  numberOfFaces or numberOfVertices is NULL.\n";
        std::exit(-4);
    }

    // -- Create Vertex Array.
    vertexDataByteSize = numberOfVertices * 6 * sizeof(GLfloat);
    vertexData = (Vertex*)calloc(numberOfVertices, sizeof(Vertex));
    if (vertexData == nullptr) {
        std::cout << "ERROR IN Shape::initialization(),  vertexData Calloc failure...\n";
        std::exit(-5);
    }

    // -- Create Faces Array.
    facesByteSize = numberOfFaces * 3 * sizeof(GLushort);
    faces = (GLushort*)calloc(numberOfFaces * 3, sizeof(GLushort));
    if (vertexData == nullptr) {
        std::cout << "ERROR IN Shape::initialization(),  faces Calloc failure...\n";
        std::exit(-5);
    }

    // -- Create Surface Normal Array.
    normalsByteSize = numberOfFaces * sizeof(glm::vec3);
    normals = (glm::vec3*)calloc(numberOfFaces, sizeof(glm::vec3));
    if (vertexData == nullptr) {
        std::cout << "ERROR IN Shape::initialization(),  normals Calloc failure...\n";
        std::exit(-5);
    }

    // -- Create OpenGL Buffer Array.
    bufferByteSize = numberOfFaces * 3 * 9 * sizeof(GLfloat);
    buffer = (GLfloat*)calloc(numberOfFaces * 3 * 9, sizeof(GLfloat));
    if (vertexData == nullptr) {
        std::cout << "ERROR IN Shape::initialization(),  buffer Calloc failure...\n";
        std::exit(-5);
    }
}

void Shape::freeArrays() {
    free(vertexData);
    free(faces);
    free(normals);
    free(buffer);
}
