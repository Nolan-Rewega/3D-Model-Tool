#version 330 core
// -- DEPTH MAP VERTEX SHADER SOURCE

in layout (location = 0) vec3 position;
in layout (location = 1) vec3 color;
in layout (location = 2) vec3 normal;

uniform mat4 lightSpaceTransform;
uniform mat4 shapeTransform;

void main(){
	gl_Position = lightSpaceTransform * shapeTransform * vec4(position, 1.0);
}