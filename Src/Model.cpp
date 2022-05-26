#include "Model.h"

Model::Model(){
	camera = new Camera(0.01f);
}
Model::~Model() {
	delete camera;
}

std::vector<Shape*> Model::getShapes(){
	return shapes;
}

void Model::addShape(int SHAPEFLAG){
	switch (SHAPEFLAG) {
	case 0:
		Shape* newShape = new Cube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
		shapes.push_back(newShape);
		break;
	}
	notifySubscribers();
}

void Model::removeShape(Shape* givenShape){
	
	notifySubscribers();
}

void Model::translateShape(Shape* givenShape, glm::vec3 translation){
	givenShape->translateShape(translation);
	notifySubscribers();
}

void Model::rotateShape(Shape* givenShape, glm::vec3 rotation, GLfloat angle){
	givenShape->rotateShape(rotation, angle);
	notifySubscribers();
}





void Model::changeCameraViewDirection(glm::vec2 delta){
	camera->updateViewDirection(glm::vec3(delta, 0.0f));
	notifySubscribers();
}

void Model::changeCameraUpVector(glm::vec3 newUpVector) {
	camera->updateUpVector(newUpVector);
	notifySubscribers();
}

void Model::changeCameraEyePosition(GLfloat leftOrBack, bool isStrafing){
	camera->updateEyePosition(leftOrBack, isStrafing);
	notifySubscribers();
}

glm::mat4 Model::getWorldtoViewMatrix(){
	return camera->getWorldToViewMatrix();
}




void Model::addSubscriber(ModelSubscriber* newSub){
	subs.push_back(newSub);
}

void Model::notifySubscribers(){
	for (ModelSubscriber* sub : subs){
		sub->modelChanged();
	}
}
