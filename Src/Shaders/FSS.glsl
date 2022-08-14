#version 330 core  

#define MAXLIGHTS 3

// -- Struct definition
//struct Material {
//	float exp; // -- specular exponent
//}

struct Light{
	// -- 0 = directed, 1 = point, 2 = spotlight;
	int type;

	// -- Lighting properties.
	vec3 position;
	vec3 direction;

	// -- Lighting components.
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// -- Soft Edge perameters.
	float phi;
	float gamma;

	// -- Attenuation perameters.
	float kc;
	float kl;
	float kq;
};

// -- Function forward definition.
vec3 directionalLight(Light light);
vec3 pointLight(Light light);
vec3 spotLight(Light light);

float calculateDiffuseLight(vec3 lightDirection);
float calculateSpecularLight(vec3 reflection, int exponent);

// -- Lighting Uniforms
uniform vec3 cameraPosition;
uniform Light lights[MAXLIGHTS];

// -- vertex shader inputs
in vec3 color;
in vec3 normal;
in vec3 position;

out vec4 FragColor;



void main(){
	vec3 light = vec3(0.0);

	// -- Add Point light values to the scene
	for(int i = 0; i < MAXLIGHTS; i++){
		// -- Directional lights.
		if( lights[i].type == 0 ){  light += directionalLight(lights[i]);  }
		
		// -- Point lights
		else if( lights[i].type == 1 ){  light += pointLight(lights[i]);  }
		
		// -- Spotlight.
		else{  light += spotLight(lights[i]);  }
	}
	
	//light *= color;
	FragColor = vec4(light, 1.0);
}

float calculateDiffuseLight(vec3 lightDirection){
	return clamp( dot( lightDirection, normal ), 0, 1 );
}

float calculateSpecularLight(vec3 lightDirection, int exponent){
	vec3 eyeVec3 = normalize( cameraPosition - position );
	vec3 halfway = normalize(lightDirection + eyeVec3);
	return pow(  max( dot( normal, halfway ), 0 ), exponent );
	//return 0.0;
}



vec3 directionalLight(Light light){
	// -- Ambient light.
	vec3 ambient    = light.ambient;

	// -- Diffuse lighting.
	vec3 lightVec3  = normalize( -light.direction);
	vec3 diffuse    = light.diffuse * calculateDiffuseLight(lightVec3);

	// -- Specular lighting.
	vec3 specular   = light.specular * calculateSpecularLight(lightVec3, 33);
	
	// -- Phong model lighting, (ambient + diffuse + specular)
	return ( ambient + diffuse + specular );
}


vec3 pointLight(Light light){
	// -- Ambient light.
	vec3 ambient    = light.ambient;

	vec3 lightVec3  = normalize( light.position - position );
	vec3 diffuse    = light.diffuse * calculateDiffuseLight(lightVec3);

	// -- Specular lighting.
	vec3 specular   = light.specular * calculateSpecularLight(lightVec3, 127);
	
	// -- Light attenuation
	float distance     = length(light.position - position);
	float attenuation  = 1.0 / (light.kc + light.kl * distance + light.kq * (distance * distance));
	
	ambient  *=  attenuation;
    diffuse  *=  attenuation;
    specular *=  attenuation;

	// -- Phong model lighting, (ambient + diffuse + specular)
	return ( ambient + diffuse + specular );
}


vec3 spotLight(Light light){
	// -- Ambient light.
	vec3 ambient    = light.ambient;

	// -- Diffuse lighting.
	vec3 lightVec3  = normalize( light.position - position );
	vec3 diffuse    = light.diffuse * calculateDiffuseLight(lightVec3);

	// -- Specular lighting.
	vec3 specular   = light.specular * calculateSpecularLight(lightVec3, 33);


	// -- Light attenuation.
	float distance     = length(light.position - position);
	float attenuation  = 1.0 / (light.kc + light.kl * distance + light.kq * (distance * distance));

	// -- Spotlight soft edges.
	float theta      = dot(lightVec3, normalize(-light.direction));
	float epsilon    = light.phi - light.gamma;
	float intensity  = clamp( (theta - light.gamma) / epsilon, 0.0, 1.0 ); 

	ambient  *=  attenuation * intensity;
    diffuse  *=  attenuation * intensity;
    specular *=  attenuation * intensity;

	return ( ambient + diffuse + specular );
}