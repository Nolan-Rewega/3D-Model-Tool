#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "ModelSubscriber.h"

#include "Shape.h"
#include "Cube.h"
#include "Triangle.h"
#include "Icosahedron.h"
#include "Plane.h"


class Model {

public:
	 Model();
	~Model();

	// -- Shape object methods.
	std::vector<Shape*> getShapes();
	void addShape(int SHAPEFLAG);
	void removeShape(Shape* givenShape);
	void translateShape(Shape* givenShape, glm::vec3 translation);
	void rotateShape(Shape* givenShape, glm::vec3 rotation, GLfloat angle);



	// -- Camera object methods.
	void zoomCamera(GLfloat dZoom);
	void rotateCamera(GLfloat dTheta, GLfloat dPhi);
	void translateCamera(glm::vec3 delta);

	glm::mat4 getWorldtoViewMatrix();

	// -- Pub-Sub methods.
	void addSubscriber(ModelSubscriber* newSub);


private:
	Camera* camera;
	std::vector<ModelSubscriber*> subs;
	std::vector<Shape*> shapes;


	// -- Pub-Sub methods.
	void notifySubscribers();
};

#endif // !MODEL_H
