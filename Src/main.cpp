#include <iostream>
#include "./MVC/Model.h"
#include "./MVC/View.h"
#include "./MVC/Controller.h"
#include "./MVC/ModelSubscriber.h"

#include "./OpenGL/GLFWObject.h"


int main() {
	GLFWObject* obj = new GLFWObject(800, 800);
	GLFWwindow* window = obj->getWindow();

	Controller* controller = new Controller();
	Model* model = new Model();
	View* view = new View(obj);

	// -- Set MVC connections.
	view->setModel(model);
	view->setController(controller);
	controller->setModel(model);
	model->addSubscriber(view);


	// -- Main Event loop.
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
	}

	delete view;
	delete model;
	delete controller;
	delete obj;
	return 0;
}

