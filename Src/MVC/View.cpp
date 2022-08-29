#include "View.h"

View::View(GLFWObject* glfwObj) : m_WIDTH(800), m_HEIGHT(800) {
	m_glfwObj = glfwObj;

	// -- Generate Shadow Depth Map framebuffer.
	glGenFramebuffers(1, &frameBufferDM);
	
	// -- Link shaders.
	Shader* VSS = new Shader("VSS.glsl", 0);
	Shader* FSS = new Shader("FSS.glsl", 1);
	Shader* DMVSS = new Shader("DMVSS.glsl", 0);
	Shader* DMFSS = new Shader("DMFSS.glsl", 1);
	Shader* LVSS = new Shader("LVSS.glsl", 0);
	Shader* LFSS = new Shader("LFSS.glsl", 1);
	
	// -- Attach shaders
	shaderProgram = new ShaderProgram();
	lightProgram = new ShaderProgram();
	lightModelProgam = new ShaderProgram();

	shaderProgram->attachShader(VSS->getShaderID());
	shaderProgram->attachShader(FSS->getShaderID());
	lightProgram->attachShader(DMVSS->getShaderID());
	lightProgram->attachShader(DMFSS->getShaderID());
	lightModelProgam->attachShader(LVSS->getShaderID());
	lightModelProgam->attachShader(LFSS->getShaderID());

	shaderProgram->linkProgram();
	lightProgram->linkProgram();
	lightModelProgam->linkProgram();

	// -- Setting camera projection matrix.
	m_projectionMat4 = glm::perspective(glm::radians(90.0f), 800.0f / 800.0f, 0.1f, 30.0f);
	//projectionMat4 = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 30.0f);
}

View::~View(){
	delete shaderProgram;
	delete lightProgram;
	delete lightModelProgam;
	glDeleteFramebuffers(1, &frameBufferDM);
	deleteBuffers();
}



// -- Draw called only when the model changes.
void View::draw(){

	// -- Generate light Depth Maps.
	//glBindFramebuffer(GL_FRAMEBUFFER, frameBufferDM);
	//for (Light* light : m_model->getLights()) {
	//	m_glfwObj->setViewPort( light->getShadowMapSize(), light->getShadowMapSize() );
	//	renderDepthMapTexture(light);
	//}


	// -- Draw the Shapes on screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_glfwObj->setViewPort(m_WIDTH, m_HEIGHT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (Shape* shape : m_model->getShapes()){
		drawShape(shape);
	}
	for (Light* light : m_model->getLights()){
		drawLight(light);
	}

	glfwSwapBuffers(m_glfwObj->getWindow());
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
	std::string members[] = { "type",     "position",  "direction",  "ambiant",
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
			else if (s == "ambiant"  ) { glUniform3fv(location, 1, &light->getAmbience()[0]         ); }
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
	glm::mat4 shapeTransform = shape->getTranslationMatrix();


	glUniformMatrix4fv(glGetUniformLocation(shader, "fullTransMat4"), 1, GL_FALSE, &fullTransform[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "shapeTransMat4"), 1, GL_FALSE, &shapeTransform[0][0]);
	glUniform3fv(glGetUniformLocation(shader, "cameraPosition"), 1, &m_model->getEyePosition()[0]);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, shape->getNumberOfBufferVertices());

	// -- delete the VAO, VBO, EBO buffers for this shape.
	deleteBuffers();
}



void View::drawLight(Light* light) {
	GLuint shader = lightModelProgam->getProgramID();
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



//void View::renderDepthMapTexture(Light* light) {
//	GLuint lp = lightProgram->getProgramID();
//	glUseProgram(lp);
//
//	// -- Set the Framebuffer's depth buffer to the light's depth map.
//	glFramebufferTexture2D(
//		GL_FRAMEBUFFER,
//		GL_DEPTH_ATTACHMENT,
//		GL_TEXTURE_2D,
//		light->getShadowMapID(),
//		0
//	);
//
//	glDrawBuffer(GL_NONE);
//	glReadBuffer(GL_NONE);
//
//	// -- Clear the newly binded depth buffer.
//	glClear(GL_DEPTH_BUFFER_BIT);
//
//	// -- Load all vertices into the buffer.
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//
//	glBindVertexArray(VAO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(
//		GL_ARRAY_BUFFER,
//		m_model->getBatchedVertexBufferSize(),
//		m_model->getBatchedVertexBuffer(),
//		GL_STATIC_DRAW
//	);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
//	glEnableVertexAttribArray(0);
//
//	glUniformMatrix4fv(glGetUniformLocation(lp, "lightSpaceTransform"), 1, GL_FALSE, &light->getLightTransform()[0][0]);
//
//	// -- Render the depth map.
//	glBindVertexArray(VAO);
//	glDrawArrays(GL_TRIANGLES, 0, m_model->getBatchedVertexBufferSize() / (3 * sizeof(GLfloat)));
//}



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
	glDeleteBuffers(1, &EBO);
}
