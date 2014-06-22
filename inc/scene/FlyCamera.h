#pragma once

#include <scene/Camera.h>

namespace syn
{

class SYNTAX_API FlyCamera : public Camera
{
	SynRTTI;

public:

	FlyCamera(float a_fSpeed = 1, const glm::vec3& a_up = glm::vec3(0, 1, 0));
	virtual ~FlyCamera();

	virtual void	update(float a_deltaTime);

	void			setSpeed(float a_speed);
	float			getSpeed() const;

	void				setUp(const glm::vec3& a_up);
	const glm::vec3&	getUp() const;

private:

	FlyCamera(const FlyCamera& a_rhs);
	FlyCamera& operator = (const FlyCamera& a_rhs);

private:

	float			m_speed;
	glm::vec3		m_up;

	bool			m_mouseButtonDown;
	double			m_lastMouseX, m_lastMouseY;
};

inline FlyCamera::FlyCamera(float a_speed /* = 1 */, const glm::vec3& a_up /* = glm::vec3(0,1,0) */)
	: m_speed(a_speed),
	m_up(a_up),
	m_mouseButtonDown(false),
	m_lastMouseX(0),
	m_lastMouseY(0)
{

}

inline FlyCamera::~FlyCamera()
{

}

inline void FlyCamera::setSpeed(float a_speed)
{
	m_speed = a_speed;
}

inline float FlyCamera::getSpeed() const
{
	return m_speed;
}

inline void FlyCamera::setUp(const glm::vec3& a_up)
{
	m_up = a_up;
}

inline const glm::vec3& FlyCamera::getUp() const
{
	return m_up;
}

} // namespace syn