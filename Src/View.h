#ifndef VIEW_H
#define VIEW_H
#include <iostream>
#include <fstream>
#include <string>

#include "Model.h"
#include "Controller.h"
#include "ModelSubscriber.h"

using namespace std;

class View : public ModelSubscriber {

public:
	 View();
	~View();

	void setModel(Model* givenModel);
	void setController(Controller* givenController);
	GLFWwindow* getWindow();
	void modelChanged();

private:
	GLuint VAO, VBO, EBO;
	GLuint vertexShader, fragmentShader, shaderProgram;
	GLFWwindow* window;

	Model* model;
	void draw();
	void drawShape(Shape* shape);

	void checkGLSLErrors(GLuint objectID, GLint ERRORCODE);
	string readShaderCode(const char* filename);
};

#endif // !VIEW_H



