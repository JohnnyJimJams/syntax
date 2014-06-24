#version 400

layout( location = 0 ) in vec4 inPosition;
layout( location = 1 ) in vec2 inTexCoord;

out vec2 texCoord;

uniform mat4 projectionView;
uniform mat4 worldTransform;

void main()
{
	texCoord = inTexCoord;
	gl_Position = projectionView * worldTransform * inPosition;
}