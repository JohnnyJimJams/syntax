#version 400

in vec4 WPosition;

layout( location = 0 ) out vec4 FragColour;

uniform samplerCube diffuseMap;

uniform float gamma;

void main()
{
	FragColour.rgb = texture( diffuseMap, normalize(WPosition.xyz) ).rgb;
//	FragColour.rgb = pow( FragColour.rgb, vec3( 1 / 2.2f ) );
	FragColour.a = 1;
}