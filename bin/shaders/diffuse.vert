#version 400

layout( location = 0 ) in vec4 inPosition;
layout( location = 1 ) in vec4 inNormal;
layout( location = 2 ) in vec2 inTexCoord;

out vec2 texCoord;
out vec4 worldPosition;
out vec4 worldNormal;

struct Camera
{
	vec4 position;

	mat4 view;
	mat4 projection;
	mat4 projectionView;
};

uniform Camera camera;

uniform mat4 worldTransform;

void main()
{
	texCoord = inTexCoord;
	worldPosition = worldTransform * inPosition;
	worldNormal = worldTransform * inNormal;
	gl_Position = camera.projectionView * worldTransform * inPosition;
}