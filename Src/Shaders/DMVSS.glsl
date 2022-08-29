#version 330 core
// -- DEPTH MAP VERTEX SHADER SOURCE

layout (location = 0) in vec3 position;

uniform mat4 lightSpaceTransform;

void main(){
	gl_Position = lightSpaceTransform * vec4(position, 1.0);
}