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

const float refractFactor = 0.975;
const float reflectFactor = 0.1;

void main()
{
	vec3 V = normalize(worldPosition.xyz - camera.position.xyz);
	vec3 N = normalize(worldNormal.xyz);

	vec3 refl = reflect( V, N );
	vec3 refr = refract( V, N, refractFactor );		
						   
	FragColour.rgb = mix( texture( environmentMap, refr ).rgb, texture( environmentMap, refl ).rgb, reflectFactor );
	
	FragColour.a = 1;
}