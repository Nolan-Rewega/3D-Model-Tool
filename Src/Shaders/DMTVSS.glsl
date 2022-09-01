#version 330 core
in layout (location = 0) vec2 in_position;
in layout (location = 1) vec2 in_texCoords;


out vec2 TexCoords;
out vec2 pos;

void main()
{             
    gl_Position = vec4(in_position.xy, 0.0, 1.0);
    TexCoords = in_texCoords;
    pos = in_position;
}  