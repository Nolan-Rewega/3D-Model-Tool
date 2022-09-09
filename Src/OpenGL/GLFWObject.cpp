#include "GLFWObject.h"

GLFWObject::GLFWObject(int width, int height){
	// Initialize GLFW
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "3DOpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);


	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//if (glfwRawMouseMotionSupported())
	//	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);


}

GLFWObject::~GLFWObject(){
	glfwDestroyWindow(window);
	glfwTerminate();
}

GLFWwindow* GLFWObject::getWindow() {
	return window;
}

void GLFWObject::setViewPort(int width, int height){
	glViewport(0, 0, width, height);
}
