
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"Model.h"


class Controller {

public:
	Controller();

	void setModel(Model* givenModel);
	void handleKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);
	void handleMouseClick(GLFWwindow* window, int button, int action, int mods);
	void handleMouseScroll(GLFWwindow* window, double xoffset, double yoffset);
	void handleMouseMove(GLFWwindow* window, double xpos, double ypos);


private:
	enum STATE { INIT, READY, DRAGGING, HALT };
	STATE currentState;
	double counter = 0;

	double PrevX, PrevY;

	Model* model;

};

#endif //!CONTROLLER_H
