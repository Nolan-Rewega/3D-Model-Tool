#version 330 core

// -- OMNIDIRECTIONAL DEPTH MAP FRAGMENT SHADER SOURCE.

in vec4 FragPos;

// -- Uniforms
uniform vec3 lightPosition;
uniform float farPlane;


// -- Manually write fragment depth to the depth buffer. 
void main()
{
    float distance = length(FragPos.xyz - lightPosition) / farPlane;
    gl_FragDepth = distance;
}  