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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//if (glfwRawMouseMotionSupported())
//	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
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
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
}




void View::draw(){
	glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (Shape* shape : model->getShapes()){
		drawShape(shape);
	}

	glfwSwapBuffers(window);
}

void View::drawShape(Shape* shape) {
	GLfloat byteStride = 6 * sizeof(GLfloat);

	// Create reference containers for the Vartex Array Object and the Vertex Buffer Object
	GLuint VAO, VBO, EBO;

	// Generate the VAO and VBO with only 1 object each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
		
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, shape->getVertexDataSizeInBytes(), shape->getVertexData(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape->getIndicesSizeInBytes(), shape->getIndices(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, byteStride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, byteStride, (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);

	// -- each row in the mat4 needs to be send individually.
	//for (int i = 2; i <= 5; i++) { // -- i = location (2 -> 5)
	//	glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float)*(i-2)*4));
	//	glEnableVertexAttribArray(i);
	//	glVertexAttribDivisor(i, 1);
	//}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glm::mat4 projectionMat4 = glm::perspective(glm::radians(90.0f), 800.0f / 800.0f, 0.1f, 10.0f);
	glm::mat4 fullBoy = projectionMat4 * model->getWorldtoViewMatrix() * shape->getTranslationMatrix() * shape->getRotationMatrix();

	GLint transformMat4Loc = glGetUniformLocation(shaderProgram, "transformMat4");
	glUniformMatrix4fv(transformMat4Loc, 1, GL_FALSE, &fullBoy[0][0]);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, shape->getIndicesSizeInBytes() / sizeof(shape->getIndices()[0]), GL_UNSIGNED_SHORT, 0);


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

	auto mouseMovement = [](GLFWwindow* window, double xpos, double ypos) {
		static_cast<Controller*>(glfwGetWindowUserPointer(window))->handleMouseMove(window, xpos, ypos);
	};


	glfwSetMouseButtonCallback(window, mouseClick);
	glfwSetKeyCallback(window, keyPress);
	glfwSetCursorPosCallback(window, mouseMovement);

}

GLFWwindow* View::getWindow(){
	return window;
}

void View::modelChanged() {
	draw();
}


