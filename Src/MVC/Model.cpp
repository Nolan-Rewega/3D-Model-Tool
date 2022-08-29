#include "Model.h"

Model::Model(){
	m_camera = new Camera(0.01f, 3.0f);
	m_totalFaces = 0;
}
Model::~Model() {
	delete m_camera;
	for (Shape* s : m_shapes) { delete s; }
	for (Light* l : m_lights) { delete l; }
	m_shapes.clear();
	m_lights.clear();
	free(m_BVB);
}




std::vector<Shape*> Model::getShapes(){
	return m_shapes;
}

GLfloat* Model::getBatchedVertexBuffer(){
	return m_BVB;
}

int Model::getBatchedVertexBufferSize(){
	return m_totalFaces * 3 * 3 * sizeof(GLfloat);
}



void Model::addShape(int SHAPEFLAG){
	Shape* newShape = nullptr;
	switch (SHAPEFLAG) {
	case 0:
		newShape = new Cube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
		m_totalFaces += 12;
		break;
	case 1:
		newShape = new Icosahedron(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
		m_totalFaces += 20;
		break;
	case 2:
		newShape = new Plane(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 0.8f), 20.0f, 20.0f);
		m_totalFaces += 2;
		break;
	case 3:
		newShape = new Tetrahedron(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), 1.0f);
		m_totalFaces += 4;
		break;
	default:
		break;
	}
	m_shapes.push_back(newShape);
	//generateBVB();
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




void Model::addLight( Light::TYPE type,
	                  glm::vec3 position,
	                  glm::vec3 direction,
	                  glm::vec3 ambience,
	                  glm::vec3 diffusion,
	                  glm::vec3 specularity,
	                  glm::vec3 attenuation,
	                  glm::vec2 softEdgeConstants )
{
	m_lights.push_back( new Light( type, 
		                           position, 
		                           direction, 
		                           ambience, 
		                           diffusion, 
		                           specularity, 
		                           attenuation, 
		                           softEdgeConstants )
	);
	notifySubscribers();
}

std::vector<Light*> Model::getLights() {
	return m_lights;
}




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


void Model::generateBVB(){
	free(m_BVB);
	m_BVB = (GLfloat*)calloc(m_totalFaces * 3 * 3, sizeof(GLfloat));
	if (m_BVB == nullptr) {
		std::cout << "ERROR IN Model::fillBVB... CALLOC nullptr.\n";
		std::exit(-82);
	}
	
	int offset = 0;
	for (Shape* s : m_shapes) {
		GLfloat* shapeBuffer = s->getShapeBuffer();

		for (int j = 0; j < s->getNumberOfBufferVertices(); j += 9) {
			m_BVB[offset + (j / 3) + 0] = shapeBuffer[j + 0];
			m_BVB[offset + (j / 3) + 1] = shapeBuffer[j + 1];
			m_BVB[offset + (j / 3) + 2] = shapeBuffer[j + 2];
		}

		offset += s->getNumberOfBufferVertices() * 3;
	}
}
