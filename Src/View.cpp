#include "View.h"

View::View(){
	// -- read shader code from a file.
	std::string VSSinput = readShaderCode("VSS.glsl");
	std::string FSSinput = readShaderCode("FSS.glsl");
	const char* VSS = VSSinput.c_str();
	const char* FSS = FSSinput.c_str();


	// Initialize GLFW
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 800, "3DOpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glViewport(0, 0, 800, 800);

//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//if (glfwRawMouseMotionSupported())
//	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shaderProgram = glCreateProgram();

	glShaderSource(vertexShader, 1, &VSS, NULL);
	glShaderSource(fragmentShader, 1, &FSS, NULL);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	checkGLSLErrors(vertexShader, 0);

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	checkGLSLErrors(shaderProgram, 1);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


}

View::~View(){
	deleteBuffers();
	glDeleteProgram(shaderProgram);
	glfwDestroyWindow(window);
	glfwTerminate();
}




void View::draw(){
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (Shape* shape : model->getShapes()){
		drawShape(shape);
		drawNormals(shape);
	}

	glfwSwapBuffers(window);
}


void View::drawShape(Shape* shape) {
	GLfloat byteStride = 9 * sizeof(GLfloat);

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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, byteStride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, byteStride, (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, byteStride, (void*)(sizeof(GLfloat) * 6));
	glEnableVertexAttribArray(2);

	// -- each row in the mat4 needs to be sent individually.
	//for (int i = 2; i <= 5; i++) { // -- i = location (2 -> 5)
	//	glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float)*(i-2)*4));
	//	glEnableVertexAttribArray(i);
	//	glVertexAttribDivisor(i, 1);
	//}

	glm::mat4 projectionMat4 = glm::perspective(glm::radians(90.0f), 800.0f / 800.0f, 0.1f, 30.0f);
	//glm::mat4 projectionMat4 = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 30.0f);
	glm::mat4 fullTransform = projectionMat4 * model->getWorldtoViewMatrix() * shape->getTranslationMatrix() * shape->getRotationMatrix();
	glm::mat4 shapeTransform = shape->getTranslationMatrix();



	glm::vec3 ambientLight = glm::vec3(0.05f, 0.05f, 0.05f);
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, -3.0f);

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "fullTransMat4"), 1, GL_FALSE, &fullTransform[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "shapeTransMat4"), 1, GL_FALSE, &shapeTransform[0][0]);

	glUniform3fv(glGetUniformLocation(shaderProgram, "ambientLight"), 1, &ambientLight[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram, "lightPosition"), 1, &lightPos[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram, "cameraPosition"), 1, &model->getEyePosition()[0]);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, shape->getNumberOfBufferVertices());

	// -- delete the VAO, VBO, EBO buffers for this shape.
	deleteBuffers();
}


void View::drawNormals(Shape* shape){
	GLfloat byteStride = 9 * sizeof(GLfloat);
	GLfloat* test = shape->debugConvertToLineNormals();
	// -- Generate the VAO, VBO, EBO for this shape.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(
		GL_ARRAY_BUFFER,
		shape->getShapeBufferSizeInBytes() * 2,
		test,
		GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, byteStride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, byteStride, (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, byteStride, (void*)(sizeof(GLfloat) * 6));
	glEnableVertexAttribArray(2);

	// -- each row in the mat4 needs to be sent individually.
	//for (int i = 2; i <= 5; i++) { // -- i = location (2 -> 5)
	//	glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float)*(i-2)*4));
	//	glEnableVertexAttribArray(i);
	//	glVertexAttribDivisor(i, 1);
	//}

	glm::mat4 projectionMat4 = glm::perspective(glm::radians(90.0f), 800.0f / 800.0f, 0.1f, 30.0f);
	glm::mat4 fullTransform = projectionMat4 * model->getWorldtoViewMatrix() * shape->getTranslationMatrix() * shape->getRotationMatrix();
	glm::mat4 shapeTransform = shape->getTranslationMatrix() * shape->getRotationMatrix();

	glm::vec3 ambientLight = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, -6.0f);

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transformMat4"), 1, GL_FALSE, &fullTransform[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelMat4"), 1, GL_FALSE, &shapeTransform[0][0]);

	glUniform3fv(glGetUniformLocation(shaderProgram, "ambientLight"), 1, &ambientLight[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram, "lightPosition"), 1, &lightPos[0]);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, shape->getNumberOfBufferVertices() * 2);

	free(test);
	
	// -- delete the VAO, VBO, EBO buffers for this shape.
	deleteBuffers();





}



void View::checkGLSLErrors(GLuint objectID, GLint ERRORCODE) {
	// -- GLSL compilation checking
	GLint status;
	GLint logLen = 0;
	GLsizei realLen;
	GLchar* buffer = (GLchar*)calloc(logLen, sizeof(GLchar));

	// -- can also pass a vector of integers instead of status.
	if (ERRORCODE == 0) {
		glGetShaderiv(objectID, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			glGetShaderiv(objectID, GL_INFO_LOG_LENGTH, &logLen);
			glGetShaderInfoLog(objectID, logLen, &realLen, buffer);
			cout << buffer << std::endl;
		}
	}
	else if (ERRORCODE == 1) {
		glGetProgramiv(objectID, GL_LINK_STATUS, &status);
		if (status != GL_TRUE) {
			glGetProgramiv(objectID, GL_INFO_LOG_LENGTH, &logLen);
			glGetProgramInfoLog(objectID, logLen, &realLen, buffer);
			cout << buffer << std::endl;
		}
	}
	free(buffer);
}


string View::readShaderCode(const char* filename) {
	string shader, line;
	ifstream inputFile(filename);
	if (!inputFile.good()) {
		cout << "FAILED TO LOAD FILE: " << filename << " IN readSHaderCode()" << endl;
		exit(-420);
	}
	while (getline(inputFile, line)) {
		shader.append(line);
		shader.append("\n");
	}
	return shader;
}

void View::deleteBuffers() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}



void View::setModel(Model* givenModel) {
	model = givenModel;
}

void View::setController(Controller* givenController){
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

GLFWwindow* View::getWindow(){
	return window;
}

void View::modelChanged() {
	draw();
}


