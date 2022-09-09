#version 330 core

in vec2 TexCoords;
in vec2 pos;

out vec4 FragColor;
 
uniform sampler2D depthMap;

void main()
{             
    float depthValue = texture(depthMap, TexCoords).r;
    FragColor = vec4(vec3(depthValue), 1.0);
}  