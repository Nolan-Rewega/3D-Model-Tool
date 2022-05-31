#include "Model.h"

Model::Model(){
	camera = new Camera(0.01f, 3.0f);
}
Model::~Model() {
	delete camera;
}

std::vector<Shape*> Model::getShapes(){
	return shapes;
}

void Model::addShape(int SHAPEFLAG){
	Shape* newShape = nullptr;
	switch (SHAPEFLAG) {
	case 0:
		newShape = new Cube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
		break;
	case 1:
		newShape = new Triangle(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f);
		break;
	case 2:
		newShape = new Icosahedron(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
		break;
	case 3:
		newShape = new Plane(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f), 20.0f, 20.0f);
		break;
	default:
		break;
	}
	shapes.push_back(newShape);
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





void Model::zoomCamera(GLfloat dZoom){
	camera->zoom(dZoom);
	notifySubscribers();
}

void Model::rotateCamera(GLfloat dTheta, GLfloat dPhi){
	camera->sphereRotation(dTheta, dPhi);
	notifySubscribers();
}

void Model::translateCamera(glm::vec3 delta) {
	camera->sphereTranslate(delta);
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
