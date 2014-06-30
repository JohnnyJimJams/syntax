#version 400

in vec4 worldNormal;
in vec4 worldPosition;

layout( location = 0 ) out vec4 FragColour;

uniform samplerCube environmentMap;

struct Camera
{
	vec4 position;

	mat4 view;
	mat4 projection;
	mat4 projectionView;
};

uniform Camera camera;

// refraction
const float objectRI = 1.333;
const float refractiveIndex = 1.0 / objectRI; 
const float fresnelFactor = ((1.0 - objectRI) * (1.0 - objectRI)) / ((1.0 + objectRI) * (1.0 + objectRI));

void main()
{
	vec3 V = normalize(worldPosition.xyz - camera.position.xyz);
	vec3 N = normalize(worldNormal.xyz);
	
	vec3 refl = reflect( V, N );
	vec3 refr = refract( V, N, refractiveIndex );	

	float fresnel = fresnelFactor + (1.0 - fresnelFactor) * pow(1.0 - dot(-V, N), 5.0);
							   
	FragColour.rgb = mix( texture( environmentMap, refr ).rgb, texture( environmentMap, refl ).rgb, fresnel );
	
	FragColour.a = 1;
}