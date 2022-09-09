#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const char* VSS, const char* FSS, const char* GSS){
	m_programID = glCreateProgram();

	// -- Attach shaders
	if (GSS != nullptr) {
		m_GSS = new Shader(GSS, 2);
		this->attachShader(m_GSS->getShaderID());
	}

    m_VSS = new Shader(VSS, 0);
	this->attachShader(m_VSS->getShaderID());
    m_FSS = new Shader(FSS, 1);
	this->attachShader(m_FSS->getShaderID());

	// -- link shaders
	this->linkProgram();
}

ShaderProgram::~ShaderProgram(){
	glDeleteProgram(m_programID);
	// -- delete shaders;
	delete m_VSS;
	delete m_FSS;
	delete m_GSS;
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