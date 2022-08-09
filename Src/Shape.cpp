#include "Shape.h"

GLfloat* Shape::getShapeBuffer(){ return buffer; }
GLuint Shape::getShapeBufferSizeInBytes(){ return bufferByteSize; }
GLuint Shape::getNumberOfBufferVertices(){ return numberOfFaces * 3; }

//GLushort* Shape::getDataIndices(){ return dataIndices; }
//GLuint Shape::getDataIndicesSizeInBytes(){ return dataIndicesByteSize; }

glm::mat4 Shape::getRotationMatrix(){ return rotationMatrix; }
glm::mat4 Shape::getTranslationMatrix(){return translationMatrix; }

void Shape::convertToBuffer(){
    int n = 0;
    int idx = 0;

    // -- Force every Shape to be of these colors. (FOR DEBUGING)
    glm::vec3 colors[7] =
    {
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    };

    // -- Loop through each face.
    for (int i = 0; i < numberOfFaces * 3; ++i) {
        buffer[idx + 0] = vertexData[faces[i]].position.x;
        buffer[idx + 1] = vertexData[faces[i]].position.y;
        buffer[idx + 2] = vertexData[faces[i]].position.z;
        buffer[idx + 3] = colors[i % 7].r;
        buffer[idx + 4] = colors[i % 7].g;
        buffer[idx + 5] = colors[i % 7].b;
        buffer[idx + 6] = normals[n].x;
        buffer[idx + 7] = normals[n].y;
        buffer[idx + 8] = normals[n].z;

        n += ((i + 1) % 3 == 0) ? 1 : 0;
        idx += 9;
    }
}

GLfloat* Shape::debugConvertToLineNormals() {
    GLfloat* normalBuffer = (GLfloat*)calloc(numberOfFaces * 3 * 2 * 9, sizeof(GLfloat));
    if (normalBuffer == nullptr) {
        std::cout << "ERROR IN Shape::debugConvertToLineNormals(), Calloc failure.\n";
        exit(-40);
    }

    int n = 0;
    int idx = 0;

    for (int i = 0; i < numberOfFaces * 3; ++i) {
        // -- First line vertex

        glm::vec3 norm = normals[n];
        glm::vec3 pos  = vertexData[faces[i]].position;
        glm::vec3 pos2 = (pos + norm);

        normalBuffer[idx + 0] = pos.x;
        normalBuffer[idx + 1] = pos.y;
        normalBuffer[idx + 2] = pos.z;
        normalBuffer[idx + 3] = 1.0f;
        normalBuffer[idx + 4] = 1.0f;
        normalBuffer[idx + 5] = 1.0f;
        normalBuffer[idx + 6] = 1.0f;
        normalBuffer[idx + 7] = 1.0f;
        normalBuffer[idx + 8] = 1.0f;


        // -- Second line vertex
        normalBuffer[idx +  9] = pos2.x;
        normalBuffer[idx + 10] = pos2.y;
        normalBuffer[idx + 11] = pos2.z;
        normalBuffer[idx + 12] = 1.0f;
        normalBuffer[idx + 13] = 1.0f;
        normalBuffer[idx + 14] = 1.0f;
        normalBuffer[idx + 15] = 1.0f;
        normalBuffer[idx + 16] = 1.0f;
        normalBuffer[idx + 17] = 1.0f;

        n += ((i + 1) % 3 == 0) ? 1 : 0;
        idx += 18;
    }

    return normalBuffer;
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
    // fillVertexData();
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

    // -- Create Faces Array.
    facesByteSize = numberOfFaces * 3 * sizeof(GLushort);
    faces = (GLushort*)calloc(numberOfFaces * 3, sizeof(GLushort));

    // -- Create Surface Normal Array.
    normalsByteSize = numberOfFaces * sizeof(glm::vec3);
    normals = (glm::vec3*)calloc(numberOfFaces, sizeof(glm::vec3));

    // -- Create OpenGL Buffer Array.
    bufferByteSize = numberOfFaces * 3 * 9 * sizeof(GLfloat);
    buffer = (GLfloat*)calloc(numberOfFaces * 3 * 9, sizeof(GLfloat));
}

void Shape::freeArrays() {
    free(vertexData);
    free(faces);
    free(normals);
    free(buffer);
}
