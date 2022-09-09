#version 330 core
// -- OMNIDIRECTION DEPTH MAP GEOMETRY SHADER SOURCES

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

// -- Uniforms
uniform mat4 LST[6];

out vec4 FragPos;

// -- Transform all incoming Triangles to each face of the cubemap.
void main(){
    for(int i = 0; i < 6; i++){
        gl_Layer = i;

        // -- Get the current in triangle's vertices
        for(int j = 0; j < 3; j++){
            FragPos = gl_in[j].gl_Position;
            gl_Position = LST[i] * FragPos;
            EmitVertex();
        }    

        EndPrimitive();
    }
}  