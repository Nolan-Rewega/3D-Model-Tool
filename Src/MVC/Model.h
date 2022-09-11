#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Camera.h"
#include "ModelSubscriber.h"

#include "../Lighting/PointLight.h"
#include "../Lighting/DirectionalLight.h"

#include "../Primitives/Shape.h"
#include "../Primitives/Cube.h"
#include "../Primitives/Icosahedron.h"
#include "../Primitives/Plane.h"
#include "../Primitives/Tetrahedron.h"


class Model {

public:
	 Model();
	~Model();

	// -- Shape object methods.
	void addShape(int SHAPEFLAG);
	void removeShape(Shape* shape);
	void translateShape(Shape* shape, glm::vec3 translation);
	void rotateShape(Shape* shape, glm::vec3 rotation, GLfloat angle);

	std::vector<Shape*> getShapes();
	
	// -- Light object methods.
	void addPointLight(
		glm::vec3 position,
		glm::vec3 ambience,
		glm::vec3 diffusion,
		glm::vec3 specularity,
		glm::vec3 attenuation,
		glm::vec2 softEdgeConstants
	);

	void addDirectionalLight(
		glm::vec3 direction,
		glm::vec3 ambience,
		glm::vec3 diffusion,
		glm::vec3 specularity
	);

	std::vector<Light*> getLights();


	// -- Camera object methods.
	void zoomCamera(GLfloat dZoom);
	void rotateCamera(GLfloat dTheta, GLfloat dPhi);
	void translateCamera(glm::vec3 delta);
	void translateLight(glm::vec3 dt);

	glm::vec3 getEyePosition();
	glm::vec3 getViewDirection();
	glm::mat4 getWorldtoViewMatrix();

	// -- Pub-Sub methods.
	void addSubscriber(ModelSubscriber* newSub);


private:
	Camera* m_camera;
	std::vector<Shape*> m_shapes;
	std::vector<Light*> m_lights;
	std::vector<ModelSubscriber*> m_subs;

	// -- Pub-Sub methods.
	void notifySubscribers();
};

#endif // !MODEL_H
