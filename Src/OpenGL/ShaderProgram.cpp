#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(){
	m_programID = glCreateProgram();
}

ShaderProgram::~ShaderProgram(){
	glDeleteProgram(m_programID);
}

GLuint ShaderProgram::getProgramID(){
	return m_programID;
}



void ShaderProgram::attachShader(GLuint shaderID){
	glAttachShader(m_programID, shaderID);
}

void ShaderProgram::linkProgram(){
	glLinkProgram(m_programID);
}



void ShaderProgram::checkGLSLErrors(GLuint objectID) {
	// -- GLSL compilation checking
	GLint status;
	GLint logLen = 0;
	GLsizei realLen;
	GLchar* buffer = (GLchar*)calloc(logLen, sizeof(GLchar));

	// -- Check program Linking.
	glGetProgramiv(objectID, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		glGetProgramiv(objectID, GL_INFO_LOG_LENGTH, &logLen);
		glGetProgramInfoLog(objectID, logLen, &realLen, buffer);
		std::cout << buffer << std::endl;
		std::exit(-3);
	}
	free(buffer);
}