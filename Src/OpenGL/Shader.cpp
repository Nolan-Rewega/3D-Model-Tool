#include "Shader.h"


Shader::Shader(const char* filename, int type) {
	// -- Read .GLSL file.
	m_filename = filename;
	std::string srcString = readShaderCode(m_filename);
	const char* src       = srcString.c_str();

	// -- Compile Shader.
	// -- temp solution,
	m_shaderID = (type == 0) ? glCreateShader(GL_VERTEX_SHADER) : glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_shaderID, 1, &src, NULL);
	glCompileShader(m_shaderID);
	checkGLSLErrors(m_shaderID);

}

Shader::~Shader(){
	glDeleteShader(m_shaderID);
}

GLuint Shader::getShaderID(){
	return m_shaderID;
}



std::string Shader::readShaderCode(const char* filename) {
	std::string shader, line;
	std::ifstream inputFile(filename);
	if (!inputFile.good()) {
		std::cout << "FAILED TO LOAD FILE: " << filename << " IN Shader::readSHaderCode()\n";
		exit(-1);
	}
	while (getline(inputFile, line)) {
		shader.append(line);
		shader.append("\n");
	}
	return shader;
}

void Shader::checkGLSLErrors(GLuint objectID) {
	// -- GLSL compilation checking
	GLint status;
	GLint logLen = 0;
	GLsizei realLen;
	GLchar* buffer = (GLchar*)calloc(logLen, sizeof(GLchar));

	// -- Check the compile status.
	glGetShaderiv(objectID, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		glGetShaderiv(objectID, GL_INFO_LOG_LENGTH, &logLen);
		glGetShaderInfoLog(objectID, logLen, &realLen, buffer);
		std::cout << "Error in Shader::checkGLSLErrors with file: " << m_filename << "\n";
		std::cout << buffer << std::endl;
		std::exit(-2);
	}
	free(buffer);
}


