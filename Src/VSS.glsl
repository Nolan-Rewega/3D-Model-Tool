#version 330 core
//VERTEX SHADER

// UNIFORMS are constant values for the whole call AND ALL INSTANCED OBJECTS;
// layout is for varrying data.

in layout (location = 0) vec3 pos;
in layout (location = 1) vec3 color;
in layout (location = 2) vec3 normal;

// -- Matrices Uniforms
uniform mat4 fullTransMat4;
uniform mat4 shapeTransMat4;

// -- Lighting Uniforms
uniform vec3 ambientLight;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

out vec3 outColor;


void main() {
	gl_Position = fullTransMat4 * vec4(pos, 1.0f);

	// -- translation to the vertex position and rotate surface normal.    
	vec3 tp = vec3(shapeTransMat4 * vec4(pos, 1.0));
	vec3 rn = vec3(shapeTransMat4 * vec4(normal, 0.0));
	// -- vertex normals, uses position vector as normal vector
	//    only works when the shapes vertex positions are defined
	//    from the origin.
	//vec3 rn = vec3(shapeTransMat4 * normalize(vec4(pos, 0.0)));

	// -- Diffuse lighting.
	vec3 lightVec3 = normalize( lightPosition - tp );
	float b = clamp( dot(lightVec3, normalize(rn)), 0, 1 );

	// -- Specular lighting.
	vec3 reflectionVec3 = reflect(-lightVec3, rn);
	vec3 eyeVec3 = normalize( cameraPosition - tp ); 
	float spec = clamp( dot(reflectionVec3, eyeVec3), 0, 1 );
	spec = pow(spec, 7);

	// -- Phong model lighting, (Ambient + Diffuse + Specular)
	outColor = (ambientLight + vec3(b,b,b) + vec3(spec,spec,spec) ) * color;
};