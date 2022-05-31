#include "Shape.h"

Vertex* Shape::getVertexData(){ return vertexData; }
GLuint Shape::getVertexDataSizeInBytes(){ return vertexDataByteSize; }

GLushort* Shape::getDataIndices(){ return dataIndices; }
GLuint Shape::getDataIndicesSizeInBytes(){ return dataIndicesByteSize; }

GLushort* Shape::getOutlineIndices(){ return outlineIndices; }
GLuint Shape::getOutlineIndicesSizeInBytes(){ return outlineIndiceByteSize; }

glm::mat4 Shape::getRotationMatrix(){ return rotationMatrix; }
glm::mat4 Shape::getTranslationMatrix(){return translationMatrix; }

void Shape::translateShape(glm::vec3 targetPos) {
    translationMatrix = glm::translate(glm::mat4(1.0f), targetPos);
    // fillVertexData();
}

void Shape::rotateShape(glm::vec3 angleVec, GLfloat angle) {
    // -- remember, move the shape to the origin then rotate.
    rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), angleVec);
}


void Shape::freeVertexData(){
    free(vertexData);
}
