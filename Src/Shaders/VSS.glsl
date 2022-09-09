#version 330 core

// -- UNIFORMS are constant values for the whole call AND ALL INSTANCED OBJECTS;

in layout (location = 0) vec3 in_pos;
in layout (location = 1) vec3 in_color;
in layout (location = 2) vec3 in_normal;

// -- Matrices Uniforms
uniform mat4 fullTransMat4;
uniform mat4 shapeTransMat4;
uniform mat4 LightSpaceTransform;

// -- Fragment shader Inputs.
out vec3 m_color;
out vec3 m_normal;
out vec3 m_position;
out vec4 m_positionLST;

void main() {
	// -- Vertex properties
	gl_Position    =   fullTransMat4 * vec4(in_pos, 1.0f);
	m_color        =   in_color;

	// -- Vertex Lighting
	m_position     =   vec3(shapeTransMat4 * vec4(in_pos, 1.0));
	m_positionLST  =   LightSpaceTransform * vec4(m_position, 1.0);
	m_normal       =   normalize( vec3(shapeTransMat4 * vec4(in_normal, 0.0)) );   // -- surface normals,
}