#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Shader {
public:
	Shader(const char* filename, int type);
	~Shader();
	
	GLuint getShaderID();

private:
	// -- Shader ID
	GLuint m_shaderID;
	const char* m_filename;

	void checkGLSLErrors(GLuint objectID);
	std::string readShaderCode(const char* filename);
};


#endif // !SHADER_H
