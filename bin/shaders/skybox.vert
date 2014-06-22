#version 400

layout( location = 0 ) in vec4 inPosition;

out vec4 WPosition;

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
	WPosition = inPosition;

	mat4 world = worldTransform;
	world[3] = camera.position;

	gl_Position = camera.projectionView * world * inPosition;
}