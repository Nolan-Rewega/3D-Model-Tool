#ifndef SHADERPRGM_H
#define SHADERPRGM_H

#include <iostream>
#include <fstream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

class ShaderProgram {
public:
	ShaderProgram(const char* VSS, const char* GSS, const char* FSS = nullptr);
	~ShaderProgram();
	
	GLuint getProgramID();

	void attachShader(GLuint shaderID);
	void linkProgram();

private:
	GLuint m_programID;
	Shader* m_VSS;
	Shader* m_GSS;
	Shader* m_FSS;

	void checkGLSLErrors(GLuint objectID);

};


#endif // !SHADERPRGM_H
