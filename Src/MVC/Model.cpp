#include "Model.h"

Model::Model(){
	m_camera = new Camera(0.01f, 3.0f);
}
Model::~Model() {
	delete m_camera;
}

std::vector<Shape*> Model::getShapes(){
	return m_shapes;
}

void Model::addShape(int SHAPEFLAG){
	Shape* newShape = nullptr;
	switch (SHAPEFLAG) {
	case 0:
		newShape = new Cube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
		break;
	case 1:
		newShape = new Icosahedron(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
		break;
	case 2:
		newShape = new Plane(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f), 20.0f, 20.0f);
		break;
	case 3:
		newShape = new Tetrahedron(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f), 1.0f);
		break;
	default:
		break;
	}
	m_shapes.push_back(newShape);
	notifySubscribers();
}

void Model::removeShape(Shape* shape) {
	notifySubscribers();
}

void Model::translateShape(Shape* shape, glm::vec3 translation) {
	shape->translateShape(translation);
	notifySubscribers();
}

void Model::rotateShape(Shape* shape, glm::vec3 rotation, GLfloat angle) {
	shape->rotateShape(rotation, angle);
	notifySubscribers();
}




//void Model::addDirectionalLight( glm::vec3 direction,
//	                             glm::vec3 ambience,
//	                             glm::vec3 diffusion,
//	                             glm::vec3 specular  )
//{
//	DirectionalLight* light = new DirectionalLight(
//		direction,
//		ambience,
//		diffusion,
//		specular
//	);
//	notifySubscribers();
//}
//
//void Model::addPointLight( glm::vec3 position,
//	                       glm::vec3 ambience,
//	                       glm::vec3 diffusion,
//	                       glm::vec3 specular,
//	                       glm::vec2 attenuationConstants ) 
//{
//	PointLight* light = new PointLight(
//		position,
//		ambience,
//		diffusion,
//		specular,
//		attenuationConstants
//	);
//	notifySubscribers();
//}
//
//void Model::addSpotLight( glm::vec3 position,
//	                      glm::vec3 direction,
//	                      glm::vec3 diffusion,
//	                      glm::vec3 specular,
//	                      glm::vec2 attenuationConstants,
//	                      glm::vec2 softnessConstants)
//{
//	SpotLight* light = new SpotLight(
//		position,
//		direction,
//		diffusion,
//		specular,
//		attenuationConstants,
//		softnessConstants
//	);
//
//	notifySubscribers();
//}






void Model::zoomCamera(GLfloat dZoom){
	m_camera->zoom(dZoom);
	notifySubscribers();
}

void Model::rotateCamera(GLfloat dTheta, GLfloat dPhi){
	m_camera->sphereRotation(dTheta, dPhi);
	notifySubscribers();
}

void Model::translateCamera(glm::vec3 delta) {
	m_camera->sphereTranslate(delta);
	notifySubscribers();
}


glm::vec3 Model::getEyePosition(){
	return m_camera->getEyePosition();
}

glm::vec3 Model::getViewDirection(){
	return m_camera->getViewDirection();
}

glm::mat4 Model::getWorldtoViewMatrix(){
	return m_camera->getWorldToViewMatrix();
}




void Model::addSubscriber(ModelSubscriber* _sub){
	m_subs.push_back(_sub);
}

void Model::notifySubscribers(){
	for (ModelSubscriber* sub : m_subs){
		sub->modelChanged();
	}
}
