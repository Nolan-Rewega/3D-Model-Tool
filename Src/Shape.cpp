#include "Shape.h"

Vertex* Shape::getVertexData(){
    return vertexData;
}

GLuint Shape::getVertexDataSizeInBytes(){
    return vertexDataByteSize;
}

GLushort* Shape::getIndices(){
    return indices;
}

GLuint Shape::getIndicesSizeInBytes(){
    return indicesByteSize;
}

glm::mat4 Shape::getRotationMatrix(){
    return rotationMatrix;
}

glm::mat4 Shape::getTranslationMatrix(){
    return translationMatrix;
}

void Shape::freeVertexData(){
    free(vertexData);
}
