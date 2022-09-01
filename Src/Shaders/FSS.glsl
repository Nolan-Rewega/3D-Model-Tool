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
	float rho;

	// -- Attenuation perameters.
	float kc;
	float kl;
	float kq;
};


// -- Function forward definition.
vec3 directionalLight(Light light);
vec3 pointLight(Light light);
vec3 spotLight(Light light);

float calculateShadow(vec4 positionLST);
float calculateDiffuseLight(vec3 lightDirection);
float calculateSpecularLight(vec3 reflection, int exponent);


// -- Uniforms
uniform vec3 cameraPosition;
uniform Light lights[MAXLIGHTS];
uniform sampler2D shadowMap;


// -- vertex shader inputs
in vec3 color;
in vec3 normal;
in vec3 position;
in vec4 positionLST;


// -- Fragment shader outputs.
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

	light *= color;
	FragColor = vec4(light, 1.0);

	// -- Gamma correction
	//float gamma = 2.2;
	//FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
}


float calculateShadow(vec4 positionLST){
	vec3 projectedPosition = positionLST.xyz / positionLST.w;
	projectedPosition = projectedPosition * 0.5 + 0.5;

	// -- fixes oversampling
	if(projectedPosition.z > 1.0){
		return 1.0;
	}

	// -- Lower is closer
	float mapDepth = texture(shadowMap, projectedPosition.xy).r; 
    float currentDepth = projectedPosition.z;
	
	// -- offset to remove shadow acne
	float offset = 0.005;

	// -- 1.0 means illuminated, 0.0 means occluded 
    float shadow = currentDepth - offset < mapDepth  ? 1.0 : 0.0;
	return shadow;
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
	// -- Shadows calculation.
	float shadow    = calculateShadow(positionLST);

	// -- Ambient light.
	vec3 ambient    = light.ambient;

	// -- Diffuse lighting.
	vec3 lightVec3  = normalize( -light.direction);
	vec3 diffuse    = light.diffuse * calculateDiffuseLight(lightVec3);

	// -- Specular lighting.
	vec3 specular   = light.specular * calculateSpecularLight(lightVec3, 33);
	
	// -- Phong model lighting, (ambient + diffuse + specular)
	return ( ambient + (shadow * (diffuse + specular)) );
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
	float epsilon    = light.phi - light.rho;
	float intensity  = clamp( (theta - light.rho) / epsilon, 0.0, 1.0 ); 

	ambient  *=  attenuation * intensity;
	diffuse  *=  attenuation * intensity;
	specular *=  attenuation * intensity;

	return ( ambient + diffuse + specular );
}