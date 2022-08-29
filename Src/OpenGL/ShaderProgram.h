#ifndef SHADERPRGM_H
#define SHADERPRGM_H

#include <iostream>
#include <fstream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


class ShaderProgram {
public:
	ShaderProgram();
	~ShaderProgram();
	
	GLuint getProgramID();

	void attachShader(GLuint shaderID);
	void linkProgram();

private:
	GLuint m_programID;

	void checkGLSLErrors(GLuint objectID);

};


#endif // !SHADERPRGM_H
