#include "Shader.h"


Shader::Shader(const char* filename, int type) {
	// -- Using filesystem with C++17.
	auto cwd  = std::filesystem::current_path();
	auto path = cwd.append("Shaders").append(filename).string();
	std::cout << path << "\n";

	// -- Read .GLSL file.
	m_filename = filename;
	std::string srcString = readShaderCode(path.c_str());
	const char* src       = srcString.c_str();

	// -- Compile Shader.
	switch (type){
	case 0:
		m_shaderID = glCreateShader(GL_VERTEX_SHADER);
		break;
	case 1:
		m_shaderID = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case 2:
		m_shaderID = glCreateShader(GL_GEOMETRY_SHADER);
		break;
	default:
		std::cout << "ERROR IN Shader::Shader(), Invalid parameter 'type'.\n";
		std::exit(-3);
		break;
	}
	
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
		std::cout << " ERROR IN Shader::readShaderCode(), FAILED TO LOAD FILE: " << m_filename << "\n";
		exit(-1);
	}
	while (getline(inputFile, line)) {
		shader.append(line).append("\n");
	}
	return shader;
}

void Shader::checkGLSLErrors(GLuint objectID) {
	// -- GLSL compilation checking
	GLint status = GL_TRUE;
	GLint logLen = 256;
	GLsizei realLen;
	GLchar* buffer = (GLchar*)calloc(logLen, sizeof(GLchar));

	// -- Check the compile status.
	glGetShaderiv(objectID, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		glGetShaderiv(objectID, GL_INFO_LOG_LENGTH, &logLen);
		glGetShaderInfoLog(objectID, logLen, &realLen, buffer);
		std::cout << "ERROR IN Shader::checkGLSLErrors() on file: " << m_filename << "\n";
		std::cout << buffer << std::endl;
		std::exit(-2);
	}
	free(buffer);
}


