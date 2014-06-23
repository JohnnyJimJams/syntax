#version 400

in vec2 texCoord;

layout( location = 0 ) out vec4 FragColour;

uniform sampler2D diffuseMap;

void main()
{
	FragColour.rgb = texture( diffuseMap, texCoord ).rgb;
	FragColour.a = 1;
}