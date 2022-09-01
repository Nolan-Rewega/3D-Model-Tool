#include "Controller.h"

Controller::Controller(){
	currentState = INIT;
}



void Controller::handleKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods){
	switch (currentState){

	case Controller::READY:
		if (action == GLFW_PRESS && mods == GLFW_MOD_CONTROL) {
			switch (key) {
			case GLFW_KEY_W:
				model->translateCamera(glm::vec3(0.0f, 0.0f, 0.5f));
				break;
			case GLFW_KEY_A:
				model->translateCamera(glm::vec3(-0.5f, 0.0f, 0.0f));
				break;
			case GLFW_KEY_S:
				model->translateCamera(glm::vec3(0.0f, 0.0f, -0.5f));
				break;
			case GLFW_KEY_D:
				model->translateCamera(glm::vec3(0.5f, 0.0f, 0.0f));
				break;
			default:
				break;
			}
		}
		break;

	case Controller::DRAGGING:
		break;

	case Controller::HALT:
		break;

	default:
		break;
	}

}



void Controller::handleMouseClick(GLFWwindow* window, int button, int action, int mods){

	switch (currentState) {

	case Controller::INIT:
		// -- light 
		//model->addLight(
		//	Light::Point,
		//	glm::vec3(0.0f, 0.0f, -9.0f),
		//	glm::vec3(0.0f, 0.0f, 0.0f),
		//	glm::vec3(0.05f, 0.05f, 0.05f),
		//	glm::vec3(0.8f, 0.8f, 0.8f),
		//	glm::vec3(1.0f, 1.0f, 1.0f),
		//	glm::vec3(1.0f, 1.0f, 0.1f),
		//	glm::cos(glm::vec2(glm::radians(0.0f), glm::radians(0.0f)))
		//);
		//model->addLight(
		//	Light::Point,
		//	glm::vec3(0.0f, 2.0f, -1.5f),
		//	glm::vec3(0.0f, 0.0f, 0.0f),
		//	glm::vec3(0.05f, 0.05f, 0.05f),
		//	glm::vec3(0.8f, 0.8f, 0.8f),
		//	glm::vec3(1.0f, 1.0f, 1.0f),
		//	glm::vec3(1.0f, 0.1f, 0.1f),
		//	glm::cos(glm::vec2(glm::radians(0.0f), glm::radians(0.0f)))
		//);
		//model->addLight(
		//	Light::Spot,
		//	glm::vec3(3.0f, 3.0f, 0.0f),
		//	glm::vec3(0.0f, -1.0f, 0.0f),
		//	glm::vec3(0.0f, 0.0f, 0.0f),
		//	glm::vec3(1.0f, 1.0f, 1.0f),
		//	glm::vec3(1.0f, 1.0f, 1.0f),
		//	glm::vec3(1.0f, 0.1f, 0.01f),
		//	glm::cos(glm::vec2(glm::radians(12.5f), glm::radians(15.0f)))
		//);
		model->addLight(
			Light::Directional,
			glm::vec3(0.0f, 4.0f, -6.0f),
			glm::vec3(0.0f, -1.0f, 1.0f),
			glm::vec3(0.1f, 0.1f, 0.1f),
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::cos(glm::vec2(glm::radians(0.0f), glm::radians(0.0f)))
		);


		// -- Cubes
		model->addShape(0);
		model->translateShape(model->getShapes()[0], glm::vec3( 2.0f, 0.0f, -3.0f));
		model->addShape(0);
		model->translateShape(model->getShapes()[1], glm::vec3( 0.0f, 0.0f, -3.0f));
		model->addShape(0);
		model->translateShape(model->getShapes()[2], glm::vec3(-2.0f, 0.0f, -3.0f));

		// -- Ico
		model->addShape(1);
		model->translateShape(model->getShapes()[3], glm::vec3( 2.0f, 2.0f, -3.0f));
		model->addShape(1);
		model->translateShape(model->getShapes()[4], glm::vec3( 0.0f, 2.0f, -3.0f));
		model->addShape(1);
		model->translateShape(model->getShapes()[5], glm::vec3(-2.0f, 2.0f, -3.0f));

		// -- Plane
		model->addShape(2);
		model->translateShape(model->getShapes()[6], glm::vec3(0.0f, -1.0f, -3.0f));

		
		
		currentState = READY;
		break;

	case Controller::READY:
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			currentState = DRAGGING;
		}
		break;
	case Controller::DRAGGING:
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			currentState = READY;
		}
		break;

	case Controller::HALT:
		//double xpos, ypos;
		//glfwGetCursorPos(window, &xpos, &ypos);
		//model->translateShape(model->getShapes()[0], glm::vec3(xpos / 800.0f, xpos / 800.0f, -3.0f));
		break;

	default:
		break;
	}
}

void Controller::handleMouseScroll(GLFWwindow* window, double xoffset, double yoffset) {
	//std::cout << "WE SCROLLING" << xoffset << "      " << yoffset << std::endl;
	switch (currentState)
	{
	case Controller::INIT:
		break;
	case Controller::READY:
		model->zoomCamera(yoffset * 0.1f);
		break;
	case Controller::DRAGGING:
		break;
	case Controller::HALT:
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
		//model->rotateShape(model->getShapes()[0], glm::vec3(1.0f, 1.0f, 0.0f), angle);
		model->rotateCamera(PrevX - xpos, PrevY - ypos);
		break;

	case Controller::HALT:
		break;

	default:
		break;
	}
	PrevX = xpos;
	PrevY = ypos;
	//angle += 1.0f;
}




void Controller::setModel(Model* givenModel) {
	model = givenModel;
}