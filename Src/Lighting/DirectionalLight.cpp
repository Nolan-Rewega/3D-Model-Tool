#include "DirectionalLight.h"

DirectionalLight::DirectionalLight( glm::vec3 direction, 
	                                glm::vec3 ambience,
	                                glm::vec3 diffusion, 
	                                glm::vec3 specularity)
{
	// -- Shadow program
	m_shadowProgram = new ShaderProgram( "DMVSS.glsl",  "DMFSS.glsl");

	// -- Set light parameters
	m_type = 0;

	m_position = glm::vec3(0.0f);
	m_direction = direction;

	m_ambient = ambience;
	m_diffuse = diffusion;
	m_specular = specularity;

	m_attenuation = glm::vec3(0.0f);
	m_softedge = glm::vec2(0.0f);

	this->setLightSpaceTransforms();

	// -- Genertate Depth Map Texture2D
	m_shadowMapSize = 1024;

	glGenTextures(1, &m_shadowMapID);
	glBindTexture(GL_TEXTURE_2D, m_shadowMapID);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_DEPTH_COMPONENT,
		m_shadowMapSize,
		m_shadowMapSize,
		0,
		GL_DEPTH_COMPONENT,
		GL_FLOAT,
		NULL
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

}

DirectionalLight::~DirectionalLight(){
	free(m_lightModel);
}

void DirectionalLight::configureDepthAttachment(){
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMapID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}


void DirectionalLight::renderDepthmap(Shape* shape, GLuint VBO, GLuint VAO){
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
	glUniformMatrix4fv(glGetUniformLocation(lp, "lightSpaceTransform"), 1, GL_FALSE, &m_lightTransforms[0][0][0]);

	// -- Render to the depth map.
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, shape->getNumberOfBufferVertices());
}

void DirectionalLight::setLightSpaceTransforms(){
	// -- Temp fuction.
	free(m_lightModel);
	m_lightTransforms.clear();

	// -- Set Light model and LST
	glm::vec3 pos = (m_position - m_direction) * glm::vec3(5.0f);
	m_lightModel = new Cube(pos, m_diffuse, 0.25f);

	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 25.0f);
	glm::mat4 lightView = glm::lookAt(pos, m_direction, glm::vec3(0.0f, 1.0f, 0.0f));
	m_lightTransforms.push_back(lightProjection * lightView);
}
