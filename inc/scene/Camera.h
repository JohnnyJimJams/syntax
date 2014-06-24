#pragma once

#include <scene/Node.h>
#include <glm/ext.hpp>

namespace syn
{

using glm::quarter_pi;

DeclareSmartPtr(Camera);

class SYNTAX_API Camera : public Node
{
	SynRTTI;

public:

	Camera(float a_fov = quarter_pi<float>(), float a_aspect = 16/9.f, float a_near = 0.1f, float a_far = 1000);
	virtual ~Camera();

	bool				isActiveCamera() const;
	void				setActive();
	
	const glm::vec4&	getForward() const;
	const glm::vec4&	getRight() const;
	const glm::vec4&	getUp() const;
	
	// set up projection, either perspective based or orthographic (good for 2D)
	void	setPerspectiveProjection(float a_fov, float a_aspect, float a_near, float a_far);
	void	setOrthographicProjection(float a_width, float a_height, float a_near, float a_far);
	void	setOrthographicProjection(float a_left, float a_right, float a_top, float a_bottom, float a_near, float a_far);

	// access to view/projection matrices
	const glm::mat4&		getView() const;
	const glm::mat4&		getProjection()	const;
	const glm::mat4&		getProjectionView() const;

	const glm::mat4&		getInverseProjection()	const;
	const glm::mat4&		getInverseProjectionView() const;

	// set up the view matrix to look at a certain target
	// also modifies the local transform for the node
	void			lookAt(const glm::vec3& a_target, const glm::vec3& a_up);
	void			lookAt(const glm::vec4& a_target, const glm::vec4& a_up);
	void			lookAtFrom(const glm::vec3& a_eye, const glm::vec3& a_target, const glm::vec3& a_up);
	void			lookAtFrom(const glm::vec4& a_eye, const glm::vec4& a_target, const glm::vec4& a_up);

	// projects a 3D point to 2D screen space (-1 <= p <= 1)
	void			worldToScreenSpace(const glm::vec4& a_world, float& a_x, float& a_y);
	void			worldToScreenSpace(const glm::vec3& a_world, float& a_x, float& a_y);

	// screen space coords are in the range [-1,1]
	void			getPickRay(float a_screenSpaceX, float a_screenSpaceY, glm::vec3& a_direction);
	void			getPickRay(float a_screenSpaceX, float a_screenSpaceY, glm::vec4& a_direction);

	// helpers
	float			getNear() const;
	float			getFar() const;

	// frustum plane helper
	struct FrustumPlane
	{
		glm::vec3	n;
		float		d;
	};
	enum FRUSTUM_PLANE
	{
		PLANE_LEFT,
		PLANE_RIGHT,
		PLANE_TOP,
		PLANE_BOTTOM,
		PLANE_NEAR,
		PLANE_FAR,

		PLANE_Count
	};
	void				getFrustumPlanes(FrustumPlane* a_planes);

	static Camera*		getActiveCamera();

protected:

	virtual void		updateGlobalTransform();

private:

	Camera(const Camera& a_rhs);
	Camera& operator = (const Camera& a_rhs);

protected:

	float		m_near;
	float		m_far;

	glm::mat4	m_view;
	glm::mat4	m_projection;
	glm::mat4	m_projectionView;

	glm::mat4	m_invProjection;
	glm::mat4	m_invProjectionView;

	static Camera*	sm_activeCamera;
};

inline bool Camera::isActiveCamera() const
{
	return sm_activeCamera == this;
}

inline void Camera::setActive()
{
	sm_activeCamera = this;
}

inline Camera* Camera::getActiveCamera()
{
	return sm_activeCamera;
}

inline const glm::vec4& Camera::getForward() const
{
	return m_globalTransform[2];
}

inline const glm::vec4& Camera::getRight() const
{
	return m_globalTransform[0];
}

inline const glm::vec4& Camera::getUp() const
{
	return m_globalTransform[1];
}

inline const glm::mat4& Camera::getView()	const
{
	return m_view;	
}

inline const glm::mat4& Camera::getProjection()	const
{
	return m_projection;	
}

inline const glm::mat4& Camera::getProjectionView()	const
{
	return m_projectionView;	
}

inline const glm::mat4& Camera::getInverseProjection()	const
{
	return m_invProjection;	
}

inline const glm::mat4& Camera::getInverseProjectionView()	const
{
	return m_invProjectionView;	
}

inline float Camera::getNear() const
{
	return m_near;
}

inline float Camera::getFar() const
{
	return m_far;
}

} // namespace syn