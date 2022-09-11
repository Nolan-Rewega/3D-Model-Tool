#include "PointLight.h"

PointLight::PointLight( glm::vec3 position,
	                    glm::vec3 ambience,
	                    glm::vec3 diffusion,
	                    glm::vec3 specularity,
	                    glm::vec3 attenuation,
	                    glm::vec2 softEdgeConstants) 
{
	// -- Set up Point Light Shadow Program
	m_shadowProgram = new ShaderProgram("ODMVSS.glsl", "ODMFSS.glsl", "ODMGSS.glsl");

	// -- Set light parameters.
	m_type = 1;

	m_position = position;
	m_direction = glm::vec3(0.0f);

	m_ambient = ambience;
	m_diffuse = diffusion;
	m_specular = specularity;

	m_attenuation = attenuation;
	m_softedge = softEdgeConstants;

	this->setLightSpaceTransforms();

	// -- Generating Lights Depth cube map.
	m_shadowMapSize = 1024;
	glGenTextures(1, &m_shadowMapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMapID);
	// -- Call glTexImage2D to gen each of the six sides of a cubemap
	for (int i = 0; i < 6; ++i) {
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0,
			GL_DEPTH_COMPONENT,
			m_shadowMapSize,
			m_shadowMapSize,
			0,
			GL_DEPTH_COMPONENT,
			GL_FLOAT,
			NULL
		);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
}

PointLight::~PointLight(){
	free(m_lightModel);
}


void PointLight::configureDepthAttachment(){
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadowMapID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}


void PointLight::renderDepthmap(Shape* shape, GLuint VBO, GLuint VAO){
	GLuint lp = m_shadowProgram->getProgramID();
	glUseProgram(lp);

	// -- Load all vertices into the buffer.
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


	// -- Uniform Setup
	glm::mat4 shapeTransform = shape->getTranslationMatrix() * shape->getRotationMatrix();
	glUniformMatrix4fv(glGetUniformLocation(lp, "shapeTransform"), 1, GL_FALSE, &shapeTransform[0][0]);
	glUniform3fv(glGetUniformLocation(lp, "lightPosition"), 1, &m_position[0]);
	glUniform1f(glGetUniformLocation(lp, "farPlane"), 25.0f);
	glUniform1i(glGetUniformLocation(lp, "cubeMap"), 0);
	for (int i = 0; i < 6; i++) {
		char uniformName[8] = { 'L', 'S', 'T', '[', i + 48, ']' };
		auto location = glGetUniformLocation(lp, uniformName);
		glUniformMatrix4fv(location, 1, GL_FALSE, &m_lightTransforms[i][0][0]);
	}

	// -- Render to the depth map.
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, shape->getNumberOfBufferVertices());

	// -- Delete buffers
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void PointLight::setLightSpaceTransforms(){
	free(m_lightModel);
	m_lightTransforms.clear();

	// -- Set Light model and LST's
	m_lightModel = new Cube(m_position, m_diffuse, 0.25f);
	glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 10.0f);

	m_lightTransforms.push_back(lightProjection * glm::lookAt(m_position, m_position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	m_lightTransforms.push_back(lightProjection * glm::lookAt(m_position, m_position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	m_lightTransforms.push_back(lightProjection * glm::lookAt(m_position, m_position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	m_lightTransforms.push_back(lightProjection * glm::lookAt(m_position, m_position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	m_lightTransforms.push_back(lightProjection * glm::lookAt(m_position, m_position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	m_lightTransforms.push_back(lightProjection * glm::lookAt(m_position, m_position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
}
