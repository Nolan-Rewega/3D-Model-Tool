#version 330 core  

#define MAXLIGHTS 3

// -- Struct definition
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

float calculateShadowDepthMap(vec4 positionLST);
float calculateShadowCubeMap(Light light, vec4 pos);

float calculateDiffuseLight(vec3 lightDirection);
float calculateSpecularLight(vec3 reflection, int exponent);


// -- Uniforms
uniform   vec3  cameraPosition;
uniform  Light  lights[MAXLIGHTS];
uniform  float  farPlane;

uniform  sampler2D    depthMap;
uniform  samplerCube  cubeMap;


// -- vertex shader inputs
in vec3 m_color;
in vec3 m_normal;
in vec3 m_position;
in vec4 m_positionLST;


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

	light *= m_color;
	FragColor = vec4(light, 1.0);

	// -- Gamma correction
	//float gamma = 2.2;
	//FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
}





float calculateShadowDepthMap(vec4 positionLST){
	vec3 projectedPosition = positionLST.xyz / positionLST.w;
	projectedPosition = projectedPosition * 0.5 + 0.5;

	// -- fixes oversampling
	if(projectedPosition.z > 1.0){ return 1.0; }

	// -- Depth values.
	float mapDepth     = texture(depthMap, projectedPosition.xy).r; 
    float currentDepth = projectedPosition.z;
	
	// -- offset to remove shadow acne
	float offset = 0.005;

	// -- 1.0 means illuminated, 0.0 means occluded 
    float shadow = currentDepth - offset < mapDepth  ? 1.0 : 0.0;
	return shadow;
}


float calculateShadowCubeMap(Light light, vec3 position){
	vec3 direction = position - light.position; 
	
	// -- Depth values.
    float mapDepth     = (texture(cubeMap, direction).r) * farPlane;
	float currentDepth = length(direction);

	// -- offset to remove shadow acne
	float offset = 0.05;

	// -- 1.0 means illuminated, 0.0 means occluded 
    float shadow = currentDepth - offset < mapDepth  ? 1.0 : 0.0;
	return shadow;
}




float calculateDiffuseLight(vec3 lightDirection){
	return clamp( dot( lightDirection, m_normal ), 0, 1 );
}


float calculateSpecularLight(vec3 lightDirection, int exponent){
	vec3 eyeVec3 = normalize( cameraPosition - m_position );
	vec3 halfway = normalize(lightDirection + eyeVec3);
	return pow(  max( dot( m_normal, halfway ), 0 ), exponent );
}




vec3 directionalLight(Light light){
	// -- Shadows calculation.
	float shadow    = calculateShadowDepthMap(m_positionLST);

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
	float shadow = calculateShadowCubeMap(light, m_position); 

	// -- Ambient light.
	vec3 ambient    = light.ambient;

	vec3 lightVec3  = normalize( light.position - m_position );
	vec3 diffuse    = light.diffuse * calculateDiffuseLight(lightVec3);

	// -- Specular lighting.
	vec3 specular   = light.specular * calculateSpecularLight(lightVec3, 127);
	
	// -- Light attenuation
	float distance     = length(light.position - m_position);
	float attenuation  = 1.0 / (light.kc + light.kl * distance + light.kq * (distance * distance));
	
	ambient  *=  attenuation;
	diffuse  *=  attenuation;
	specular *=  attenuation;

	// -- Phong model lighting, (ambient + diffuse + specular)
	return ( ambient + (shadow * (diffuse + specular)) ); 
}


vec3 spotLight(Light light){
	// -- Ambient light.
	vec3 ambient    = light.ambient;

	// -- Diffuse lighting.
	vec3 lightVec3  = normalize( light.position - m_position );
	vec3 diffuse    = light.diffuse * calculateDiffuseLight(lightVec3);

	// -- Specular lighting.
	vec3 specular   = light.specular * calculateSpecularLight(lightVec3, 33);


	// -- Light attenuation.
	float distance     = length(light.position - m_position);
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