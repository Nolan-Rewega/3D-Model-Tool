#version 330 core
//VERTEX SHADER

// UNIFORMS are constant values for the whole call AND ALL INSTANCED OBJECTS;
// layout is for varrying data.

in layout (location = 0) vec3 pos;
in layout (location = 1) vec3 color;

uniform mat4 transformMat4;

out vec3 outColor;


void main() {
	// -- apply transformations to coords.
	gl_Position = transformMat4 * vec4(pos, 1.0f);
	outColor = color; 
};