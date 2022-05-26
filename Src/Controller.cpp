#include "Controller.h"

Controller::Controller(){
	currentState = READY;
	angle = 0.0f;
}



void Controller::handleKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods){
	switch (currentState){

	case Controller::READY:
		break;

	case Controller::DRAGGING:
		break;

	case Controller::HALT:
		switch (key){
		case GLFW_KEY_W:
			model->changeCameraEyePosition(1.0f, false);
			break;
		case GLFW_KEY_A:
			model->changeCameraEyePosition(-1.0f, true);
			break;
		case GLFW_KEY_S:
			model->changeCameraEyePosition(-1.0f, false);
			break;
		case GLFW_KEY_D:
			model->changeCameraEyePosition(1.0f, true);
			break;
		default:
			break;
		}

		break;

	default:
		break;
	}

}

void Controller::handleMouseClick(GLFWwindow* window, int button, int action, int mods){
	std::cout << "CLICKED SETUP \n";
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	switch (currentState) {

	case Controller::READY:
		model->addShape(0);
		model->addShape(0);
		model->translateShape(model->getShapes()[0], glm::vec3(0.0f, 0.0f, -3.0f));
		model->translateShape(model->getShapes()[1], glm::vec3(-1.0f, -0.5f, -1.0f));
		currentState = HALT;
		break;

	case Controller::DRAGGING:
		break;

	case Controller::HALT:
		model->translateShape(model->getShapes()[0], glm::vec3(xpos / 800.0f, xpos / 800.0f, -3.0f));
		break;

	default:
		break;
	}
}

void Controller::handleMouseMove(GLFWwindow* window, double xpos, double ypos){
	switch (currentState) {
	case Controller::READY:
		break;

	case Controller::DRAGGING:
		break;

	case Controller::HALT:
		model->rotateShape(model->getShapes()[0], glm::vec3(1.0f, 1.0f, 0.0f), angle);
		model->changeCameraViewDirection(glm::vec2(PrevX - xpos, PrevY - ypos));
		break;

	default:
		break;
	}
	PrevX = xpos;
	PrevY = ypos;
	angle += 1.0f;
}


void Controller::setModel(Model* givenModel) {
	model = givenModel;
}