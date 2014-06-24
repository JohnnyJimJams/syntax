#version 400

in vec2 texCoord;
in vec4 worldNormal;
in vec4 worldPosition;

layout( location = 0 ) out vec4 FragColour;

uniform sampler2D diffuseMap;
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
const float reflectFactor = 0.1f;
const float glassFactor = 0.7f;

void main()
{
	vec3 refl = reflect( normalize(worldPosition.xyz - camera.position.xyz), worldNormal.xyz );
	vec3 refr = refract( normalize(worldPosition.xyz - camera.position.xyz), worldNormal.xyz, refractFactor );

							   
	FragColour.rgb = mix( mix( texture( diffuseMap, texCoord ).rgb, 
							   texture( environmentMap, refr ).rgb, glassFactor ), 
						  texture( environmentMap, refl ).rgb, reflectFactor ) ;
	
	FragColour.a = 1;
}