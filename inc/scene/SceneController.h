#pragma once

#include <RefObject.h>
#include <glm/glm.hpp>

namespace syn
{

class SceneObject;

DeclareSmartPtr(SceneController);

class SYNTAX_API SceneController : public RefObject
{
public:

	SceneController() {}
	virtual ~SceneController() {}

	virtual void	update( SceneObject* a_object, float a_deltaTime ) = 0;
};

class SYNTAX_API RotationController : public SceneController
{
public:

	RotationController( float a_angle, const glm::vec3& a_axis ) : m_angle(a_angle), m_axis(a_axis) {}
	RotationController( float a_angle, float a_x, float a_y, float a_z ) : m_angle(a_angle), m_axis(a_x,a_y,a_z) {}
	virtual ~RotationController() {}

	virtual void	update( SceneObject* a_object, float a_deltaTime );

	float		m_angle;
	glm::vec3	m_axis;
};

} // namespace syn