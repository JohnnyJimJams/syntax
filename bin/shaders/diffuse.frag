#version 400

in vec2 texCoord;
in vec4 worldNormal;
in vec4 worldPosition;

layout( location = 0 ) out vec4 FragColour;

uniform samplerCube diffuseMap;
struct Camera
{
	vec4 position;

	mat4 view;
	mat4 projection;
	mat4 projectionView;
};

uniform Camera camera;

void main()
{
	vec3 refl = reflect( normalize(worldPosition.xyz - camera.position.xyz), worldNormal.xyz );
	vec3 refr = refract( normalize(worldPosition.xyz - camera.position.xyz), worldNormal.xyz, 1.1f );

	FragColour.rgb = mix( texture( diffuseMap, refl ).rgb, texture( diffuseMap, refr ).rgb, 0.9f );
	FragColour.a = 1;
}