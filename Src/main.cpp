#include <iostream>
#include "Model.h"
#include "View.h"
#include "Controller.h"
#include "ModelSubscriber.h"

int main() {
	Controller* controller = new Controller();
	Model* model = new Model();
	View* view = new View();

	// -- Set MVC connections.
	view->setModel(model);
	view->setController(controller);
	controller->setModel(model);
	model->addSubscriber(view);

	GLFWwindow* window = view->getWindow();

	// -- Main Event loop.
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
	}

	delete view;
	delete model;
	delete controller;
	return 0;
}

