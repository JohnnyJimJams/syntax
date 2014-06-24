#include <scene/Camera.h>
#include <glm/ext.hpp>

namespace syn
{

SynRTTIDerived(Camera,Node);

Camera* Camera::sm_activeCamera = nullptr;

Camera::Camera(	float a_fov /* = quarter_pi<float>() */, float a_aspect /* = 16/9.f */, 
				float a_near /* = 0.1f */, float a_far /* = 100 */)
{
	setPerspectiveProjection(a_fov,a_aspect,a_near,a_far);
}

Camera::~Camera()
{

}

void Camera::setPerspectiveProjection(float a_fov, float a_aspect, float a_near, float a_far)
{
	m_near = a_near;
	m_far = a_far;

	m_projection = glm::perspective(a_fov,a_aspect,a_near,a_far);
	m_projectionView = m_projection * m_view;

	m_invProjection = glm::inverse(m_projection);
	m_invProjectionView = glm::inverse(m_projectionView);
}

void Camera::setOrthographicProjection(float a_width, float a_height, float a_near, float a_far)
{
	m_near = a_near;
	m_far = a_far;
	m_projection = glm::ortho(a_width*-0.5f, a_width*0.5f, a_height*-0.5f, a_height*0.5f, a_near, a_far);
	m_projectionView = m_projection * m_view;

	m_invProjection = glm::inverse(m_projection);
	m_invProjectionView = glm::inverse(m_projectionView);
}

void Camera::setOrthographicProjection(float a_left, float a_right, float a_top, float a_bottom, float a_near, float a_far)
{
	m_near = a_near;
	m_far = a_far;
	m_projection = glm::ortho(a_left, a_right, a_top, a_bottom, a_near, a_far);
	m_projectionView = m_projection * m_view;

	m_invProjection = glm::inverse(m_projection);
	m_invProjectionView = glm::inverse(m_projectionView);
}

void Camera::lookAt(const glm::vec3& a_target, const glm::vec3& a_up)
{
	// create an orthogonal matrix frame that is aimed at the target
	glm::vec3 zAxis = glm::normalize(m_localTransform[3].xyz - a_target);
	glm::vec3 xAxis = glm::normalize(glm::cross(a_up, zAxis));
	glm::vec3 yAxis = glm::normalize(glm::cross(zAxis, xAxis));

	m_localTransform[0] = glm::vec4(xAxis, 0);
	m_localTransform[1] = glm::vec4(yAxis, 0);
	m_localTransform[2] = glm::vec4(zAxis, 0);

	updateGlobalTransform();
}

void Camera::lookAt(const glm::vec4& a_target, const glm::vec4& a_up)
{
	// create an orthogonal matrix frame that is aimed at the target
	glm::vec3 zAxis = glm::normalize(m_localTransform[3] - a_target).xyz;
	glm::vec3 xAxis = glm::normalize(glm::cross(a_up.xyz(), zAxis));
	glm::vec3 yAxis = glm::normalize(glm::cross(zAxis, xAxis));

	m_localTransform[0] = glm::vec4(xAxis, 0);
	m_localTransform[1] = glm::vec4(yAxis, 0);
	m_localTransform[2] = glm::vec4(zAxis, 0);

	updateGlobalTransform();
}

void Camera::lookAtFrom(const glm::vec3& a_eye,
	const glm::vec3& a_target,
	const glm::vec3& a_up)
{
	// create an orthogonal matrix frame that is aimed at the target from the eye position
	glm::vec3 zAxis = glm::normalize(a_eye - a_target);
	glm::vec3 xAxis = glm::normalize(glm::cross(a_up, zAxis));
	glm::vec3 yAxis = glm::normalize(glm::cross(zAxis, xAxis));

	m_localTransform[0] = glm::vec4(xAxis, 0);
	m_localTransform[1] = glm::vec4(yAxis, 0);
	m_localTransform[2] = glm::vec4(zAxis, 0);
	m_localTransform[3] = glm::vec4(a_eye, 1);

	updateGlobalTransform();
}

void Camera::lookAtFrom(const glm::vec4& a_eye,
	const glm::vec4& a_target,
	const glm::vec4& a_up)
{
	// create an orthogonal matrix frame that is aimed at the target from the eye position
	glm::vec3 zAxis = glm::normalize(a_eye - a_target).xyz;
	glm::vec3 xAxis = glm::normalize(glm::cross(a_up.xyz(), zAxis));
	glm::vec3 yAxis = glm::normalize(glm::cross(zAxis, xAxis));

	m_localTransform[0] = glm::vec4(xAxis, 0);
	m_localTransform[1] = glm::vec4(yAxis, 0);
	m_localTransform[2] = glm::vec4(zAxis, 0);
	m_localTransform[3] = a_eye;

	updateGlobalTransform();
}

void Camera::updateGlobalTransform()
{
	Node::updateGlobalTransform();

	// remove scale
	glm::mat4 view = m_globalTransform;
	view[0] = glm::normalize(view[0]);
	view[1] = glm::normalize(view[1]);
	view[2] = glm::normalize(view[2]);

	m_view = glm::inverse(view);
	m_projectionView = m_projection * m_view;

	m_invProjectionView = glm::inverse(m_projectionView);
}

void Camera::worldToScreenSpace(const glm::vec3& a_world, float& a_x, float& a_y)
{
	glm::vec4 screenSpace = m_projectionView * glm::vec4(a_world, 1);
	a_x = screenSpace.x / screenSpace.w;
	a_y = screenSpace.y / screenSpace.w;
}

void Camera::worldToScreenSpace(const glm::vec4& a_world, float& a_x, float& a_y)
{
	glm::vec4 screenSpace = m_projectionView * a_world;
	a_x = screenSpace.x / screenSpace.w;
	a_y = screenSpace.y / screenSpace.w;
}

// screen space coords are in the range [-1,1]
void Camera::getPickRay(float a_screenSpaceX, float a_screenSpaceY, glm::vec3& a_direction)
{
	a_screenSpaceX /= m_projection[0][0];
	a_screenSpaceY /= m_projection[1][1];

	a_direction.x = a_screenSpaceX;
	a_direction.y = a_screenSpaceY;
	a_direction.z = 1.0f;
	
	a_direction = glm::normalize(m_globalTransform * glm::vec4(a_direction, 0)).xyz();
}

// screen space coords are in the range [-1,1]
void Camera::getPickRay(float a_screenSpaceX, float a_screenSpaceY, glm::vec4& a_direction)
{
	a_screenSpaceX /= m_projection[0][0];
	a_screenSpaceY /= m_projection[1][1];

	a_direction.x = a_screenSpaceX;
	a_direction.y = a_screenSpaceY;
	a_direction.z = 1.0f;
	a_direction.w = 0;
	
	a_direction = glm::normalize(m_globalTransform * a_direction);
}

void Camera::getFrustumPlanes(FrustumPlane* a_planes)
{
	// left clipping plane
	a_planes[PLANE_LEFT].n = glm::vec3(m_projectionView[0][3] + m_projectionView[0][0],
		m_projectionView[1][3] + m_projectionView[1][0],
		m_projectionView[2][3] + m_projectionView[2][0] );
	a_planes[PLANE_LEFT].d = m_projectionView[3][3] + m_projectionView[3][0];

	// right clipping plane
	a_planes[PLANE_RIGHT].n = glm::vec3(m_projectionView[0][3] - m_projectionView[0][0],
		m_projectionView[1][3] - m_projectionView[1][0],
		m_projectionView[2][3] - m_projectionView[2][0] );
	a_planes[PLANE_RIGHT].d = m_projectionView[3][3] - m_projectionView[3][0];

	// top clipping plane
	a_planes[PLANE_TOP].n = glm::vec3(m_projectionView[0][3] - m_projectionView[0][1],
		m_projectionView[1][3] - m_projectionView[1][1],
		m_projectionView[2][3] - m_projectionView[2][1] );
	a_planes[PLANE_TOP].d = m_projectionView[3][3] - m_projectionView[3][1];

	// bottom clipping plane
	a_planes[PLANE_BOTTOM].n = glm::vec3(m_projectionView[0][3] + m_projectionView[0][1],
		m_projectionView[1][3] + m_projectionView[1][1],
		m_projectionView[2][3] + m_projectionView[2][1] );
	a_planes[PLANE_BOTTOM].d = m_projectionView[3][3] + m_projectionView[3][1];

	// far clipping plane
	a_planes[PLANE_FAR].n = glm::vec3(m_projectionView[0][3] - m_projectionView[0][2],
		m_projectionView[1][3] - m_projectionView[1][2],
		m_projectionView[2][3] - m_projectionView[2][2] );
	a_planes[PLANE_FAR].d = m_projectionView[3][3] - m_projectionView[3][2];

	// near clipping plane
	a_planes[PLANE_NEAR].n = glm::vec3(m_projectionView[0][2],
		m_projectionView[1][2],
		m_projectionView[2][2] );
	a_planes[PLANE_NEAR].d = m_projectionView[3][2];
}

} // namespace syn