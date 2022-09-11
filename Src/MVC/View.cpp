#include "View.h"

View::View(GLFWObject* glfwObj) : m_WIDTH(800), m_HEIGHT(800) {
	m_glfwObj = glfwObj;

	// -- Generate Shadow Depth Map framebuffer.
	glGenFramebuffers(1, &frameBufferDM);
	
	// -- Create shader programs
	shaderProgram      = new ShaderProgram(   "VSS.glsl",    "FSS.glsl");
	lightModelProgram  = new ShaderProgram(  "LVSS.glsl",   "LFSS.glsl");
	debug              = new ShaderProgram("TDMVSS.glsl", "TDMFSS.glsl");


	// -- Setting camera projection matrix.
	m_projectionMat4 = glm::perspective(glm::radians(90.0f), 800.0f / 800.0f, 0.1f, 30.0f);
}

View::~View(){
	delete shaderProgram;
	delete lightModelProgram;
	delete debug;
	glDeleteFramebuffers(1, &frameBufferDM);
	deleteBuffers();
}



// -- Draw called only when the model changes.
void View::draw(){

	// -- Generate light Depth Maps.
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferDM);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (Light* light : m_model->getLights()) {
		std::cout << light->getShadowMapID() << "\n";
		m_glfwObj->setViewPort(light->getShadowMapSize(), light->getShadowMapSize());

		// -- Set the Framebuffer's depth buffer to the light's depth map.
		light->configureDepthAttachment();
		glClear(GL_DEPTH_BUFFER_BIT);

		// -- Then render each shape to the light's depth map
		for (Shape* shape : m_model->getShapes()) { 
			light->renderDepthmap(shape, VBO, VAO);
		}

		// --  Activate light's Depth map texture for shadow rendering.
		glActiveTexture(GL_TEXTURE0 + light->getShadowMapID());
		GLuint texType = (light->getType() == 0) ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP;
		glBindTexture(texType, light->getShadowMapID());
	}


	// -- Draw the Shapes and light object models on screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_glfwObj->setViewPort(m_WIDTH, m_HEIGHT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// -- debug draw textbuffer
	//drawDepthMap();


	for (Shape* shape : m_model->getShapes()){
		drawShape(shape);
	}
	for (Light* light : m_model->getLights()){
		drawLight(light);
	}

	glfwSwapBuffers(m_glfwObj->getWindow());
}


// -- DEBUG FUNCTION.
void View::drawDepthMap() {
	GLuint shader = debug->getProgramID();
	glUseProgram(shader);

	GLfloat vertices[] = {
		// --  x     y,    u    v 
		    -1.0f,  1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,

			-1.0f,  1.0f, 0.0f, 1.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 1.0f, 1.0f,
	};


	// -- Generate the VAO, VBO, EBO for this shape.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(vertices),
		&vertices,
		GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// -- delete the VAO, VBO, EBO buffers for this shape.
	deleteBuffers();
}



void View::drawShape(Shape* shape) {
	GLuint shader = shaderProgram->getProgramID();
	glUseProgram(shader);

	// -- Generate the VAO, VBO, EBO for this shape.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(
		GL_ARRAY_BUFFER,
		shape->getShapeBufferSizeInBytes(),
		shape->getShapeBuffer(),
		GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 6));
	glEnableVertexAttribArray(2);
	
	// -- Uniform loading.
	std::string members[] = { "type",     "position",  "direction",  "ambient",
		                      "diffuse",  "specular",  "kc",         "kl",  
		                      "kq",       "phi",       "rho"                    };
	// -- THIS ONLY WORKS UPTO 10 LIGHTS
	for (int i = 0; i < m_model->getLights().size(); ++i) {
		Light* light = m_model->getLights()[i];

		for (std::string s : members) {
			// -- largerst variable name is 20 characters.
			char uniformName[20] = { 'l', 'i', 'g', 'h', 't', 's', '[', i + 48, ']', '.' };
			memcpy(&uniformName[10], s.c_str(), s.length() + 1);
			
			auto location = glGetUniformLocation(shader, uniformName);

			if      (s == "type"     ) { glUniform1i (location,     light->getType()                ); }
			else if (s == "position" ) { glUniform3fv(location, 1, &light->getPosition()[0]         ); }
			else if (s == "direction") { glUniform3fv(location, 1, &light->getDirection()[0]        ); }
			else if (s == "ambient"  ) { glUniform3fv(location, 1, &light->getAmbience()[0]         ); }
			else if (s == "diffuse"  ) { glUniform3fv(location, 1, &light->getDiffusion()[0]        ); }
			else if (s == "specular" ) { glUniform3fv(location, 1, &light->getSpecularity()[0]      ); }
			else if (s == "kc"       ) { glUniform1f (location, light->getAttenuationConstants()[0] ); }
			else if (s == "kl"       ) { glUniform1f (location, light->getAttenuationConstants()[1] ); }
			else if (s == "kq"       ) { glUniform1f (location, light->getAttenuationConstants()[2] ); }
			else if (s == "phi"      ) { glUniform1f (location, light->getSoftEdgeConstants()[0]    ); }
			else if (s == "rho"      ) { glUniform1f (location, light->getSoftEdgeConstants()[1]    ); }
		}
	}

	glm::mat4 fullTransform = m_projectionMat4 * m_model->getWorldtoViewMatrix() * shape->getTranslationMatrix() * shape->getRotationMatrix();
	glm::mat4 shapeTransform = shape->getTranslationMatrix() * shape->getRotationMatrix();
	glm::mat4 LST = m_model->getLights()[0]->getLightTransforms()[0];

	glUniform1f(glGetUniformLocation(shader, "farPlane"), 25.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader, "fullTransMat4"), 1, GL_FALSE, &fullTransform[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "shapeTransMat4"), 1, GL_FALSE, &shapeTransform[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "LightSpaceTransform"), 1, GL_FALSE, &LST[0][0]);
	glUniform3fv(glGetUniformLocation(shader, "cameraPosition"), 1, &m_model->getEyePosition()[0]);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, shape->getNumberOfBufferVertices());

	// -- delete the VAO, VBO, EBO buffers for this shape.
	deleteBuffers();
}



void View::drawLight(Light* light) {
	GLuint shader = lightModelProgram->getProgramID();
	glUseProgram(shader);

	// -- Generate the VAO, VBO, EBO for this shape.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(
		GL_ARRAY_BUFFER,
		light->getLightModel()->getShapeBufferSizeInBytes(),
		light->getLightModel()->getShapeBuffer(),
		GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 6));
	glEnableVertexAttribArray(2);

	glm::mat4 fullTransform = m_projectionMat4 * m_model->getWorldtoViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(shader, "fullTransMat4"), 1, GL_FALSE, &fullTransform[0][0]);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, light->getLightModel()->getNumberOfBufferVertices());

	// -- delete the VAO, VBO, EBO buffers for this shape.
	deleteBuffers();
}





void View::setModel(Model* model) {
	m_model = model;
}

// -- setController() is only called once.
void View::setController(Controller* givenController){
	GLFWwindow* window = m_glfwObj->getWindow();

	// -- Event Handling
	glfwSetWindowUserPointer(window, givenController);

	auto mouseClick = [](GLFWwindow* window, int button, int action, int mods) {
		static_cast<Controller*>(glfwGetWindowUserPointer(window))->handleMouseClick(window, button, action, mods);
	};

	auto keyPress = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		static_cast<Controller*>(glfwGetWindowUserPointer(window))->handleKeyPressed(window, key, scancode, action, mods);
	};

	auto mouseScroll = [](GLFWwindow* window, double xoffset, double yoffset) {
		static_cast<Controller*>(glfwGetWindowUserPointer(window))->handleMouseScroll(window, xoffset, yoffset);
	};

	auto mouseMovement = [](GLFWwindow* window, double xpos, double ypos) {
		static_cast<Controller*>(glfwGetWindowUserPointer(window))->handleMouseMove(window, xpos, ypos);
	};


	glfwSetMouseButtonCallback(window, mouseClick);
	glfwSetKeyCallback(window, keyPress);
	glfwSetScrollCallback(window, mouseScroll);
	glfwSetCursorPosCallback(window, mouseMovement);
}


void View::modelChanged() {
	draw();
}


void View::deleteBuffers() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
