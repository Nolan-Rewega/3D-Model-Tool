#version 330 core

// -- LIGHT MODEL VERTEX SHADER SOURCE CODE.

in layout (location = 0) vec3 in_pos;
in layout (location = 1) vec3 in_color;
in layout (location = 2) vec3 in_normal;

// -- Uniforms
uniform mat4 fullTransMat4;

// -- Fragment shader Inputs.
out vec3 color;

void main() {
	gl_Position  =   fullTransMat4 * vec4(in_pos, 1.0f);
	color        =   in_color;
};