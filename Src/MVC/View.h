#ifndef VIEW_H
#define VIEW_H
#include <iostream>
#include <fstream>
#include <string>

#include "Model.h"
#include "Controller.h"
#include "ModelSubscriber.h"

#include "GLFWObject.h"
#include "Shader.h"
#include "ShaderProgram.h"


class View : public ModelSubscriber {

public:
	 View(GLFWObject* windowObj);
	~View();

	void setModel(Model* model);
	void setController(Controller* controller);

private:
	const int m_WIDTH;
	const int m_HEIGHT;

	GLFWObject* m_glfwObj;

	glm::mat4 m_projectionMat4;
	Model* m_model;
	
	GLuint VAO, VBO, EBO;

	ShaderProgram* shaderProgram; 
	ShaderProgram* lightProgram;
	ShaderProgram* lightModelProgam;
	ShaderProgram* debug;

	GLuint frameBufferDM;

	void draw();
	void drawShape(Shape* shape);
	void drawLight(Light* light);
	void drawDepthMap();

	void renderDepthMapTexture(Light* light, Shape* shape);

	void modelChanged();
	void deleteBuffers();
};

#endif // !VIEW_H



