#ifndef GLFWOBJECT_H
#define GLFWOBJECT_H

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GLFWObject {
public:

	GLFWObject(int width, int height);
	~GLFWObject();

	GLFWwindow* getWindow();
	void setViewPort(int width, int height);


private:
	GLFWwindow* window;
};


#endif // !GLFWOBJECT_H

