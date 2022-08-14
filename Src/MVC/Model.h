#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "ModelSubscriber.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "Shape.h"
#include "Cube.h"
#include "Icosahedron.h"
#include "Plane.h"
#include "Tetrahedron.h"


class Model {

public:
	 Model();
	~Model();

	// -- Shape object methods.
	std::vector<Shape*> getShapes();
	void addShape(int SHAPEFLAG);
	void removeShape(Shape* shape);
	void translateShape(Shape* shape, glm::vec3 translation);
	void rotateShape(Shape* shape, glm::vec3 rotation, GLfloat angle);

	// -- Lighting object methods.
	//void addDirectionalLight(
	//	glm::vec3 direction,
	//	glm::vec3 ambience,
	//	glm::vec3 diffusion,
	//	glm::vec3 specular
	//);

	//void addPointLight(
	//	glm::vec3 position,
	//	glm::vec3 ambience,
	//	glm::vec3 diffusion,
	//	glm::vec3 specular,
	//	glm::vec2 attenuationConstants
	//);

	//void addSpotLight(
	//	glm::vec3 position,
	//	glm::vec3 direction,
	//	glm::vec3 diffusion,
	//	glm::vec3 specular,
	//	glm::vec2 attenuationConstants,
	//	glm::vec2 softnessConstants
	//);


	// -- Camera object methods.
	void zoomCamera(GLfloat dZoom);
	void rotateCamera(GLfloat dTheta, GLfloat dPhi);
	void translateCamera(glm::vec3 delta);

	glm::vec3 getEyePosition();
	glm::vec3 getViewDirection();
	glm::mat4 getWorldtoViewMatrix();

	// -- Pub-Sub methods.
	void addSubscriber(ModelSubscriber* newSub);


private:
	Camera* m_camera;
	std::vector<ModelSubscriber*> m_subs;
	std::vector<Shape*> m_shapes;


	// -- Pub-Sub methods.
	void notifySubscribers();
};

#endif // !MODEL_H
