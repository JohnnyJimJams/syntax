#include <scene/SceneObject.h>
#include <scene/SceneNode.h>
#include <glm/ext.hpp>

namespace syn
{

SynRTTIDerived(SceneObject,Object);

SceneObject::SceneObject()
	: m_parent(nullptr),
	m_localTransform(glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1)),
	m_globalTransform(glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1)),
	m_visible(true)
{
}

SceneObject::SceneObject(const char* a_name)
	: Object(a_name),
	m_parent(nullptr),
	m_localTransform(glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)),
	m_globalTransform(glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)),
	m_visible(true)
{
}

SceneObject::~SceneObject()
{

}

void SceneObject::render()
{
/*	if (m_bVisible == true)
	{
		if (g_bDebugRenderTransforms)
		{
			if (Visualiser::GetSingleton() != nullptr)
				Visualiser::GetSingleton()->AddTransform(m_mGlobalTransform);
		}

		if (g_bDebugRenderHierarchy && 
			m_pParent != nullptr)
		{
			if (Visualiser::GetSingleton() != nullptr)
				Visualiser::GetSingleton()->AddLine(m_pParent->GetGlobalTranslation(),GetGlobalTranslation(),float4(1,1,0,1));
		}
	}*/
}

void SceneObject::setLocalTransform(const glm::mat4& a_transform)
{
	m_localTransform = a_transform;
	updateGlobalTransform();
}

void SceneObject::setLocalTranslation(const glm::vec3& a_translation)
{
	m_localTransform[3].xyz = a_translation;
	updateGlobalTransform();
}

void SceneObject::setLocalTranslation(const glm::vec4& a_translation)
{
	m_localTransform[3] = a_translation;
	updateGlobalTransform();
}

void SceneObject::setLocalTranslation(float a_x, float a_y, float a_z)
{
	m_localTransform[3][0] = a_x;
	m_localTransform[3][1] = a_y;
	m_localTransform[3][2] = a_z;
	updateGlobalTransform();
}

void SceneObject::translateLocal(const glm::vec3& a_translation)
{
	m_localTransform[3].xyz += a_translation;
	updateGlobalTransform();
}

void SceneObject::translateLocal(const glm::vec4& a_translation)
{
	m_localTransform[3] += a_translation;
	updateGlobalTransform();
}

void SceneObject::translateLocal(float a_x, float a_y, float a_z)
{
	m_localTransform[3][0] += a_x;
	m_localTransform[3][1] += a_y;
	m_localTransform[3][2] += a_z;
	updateGlobalTransform();
}

void SceneObject::setLocalScale(float a_uniformScale)
{
	m_localTransform[0] = glm::normalize(m_localTransform[0]) * a_uniformScale;
	m_localTransform[1] = glm::normalize(m_localTransform[1]) * a_uniformScale;
	m_localTransform[2] = glm::normalize(m_localTransform[2]) * a_uniformScale;
	updateGlobalTransform();
}

void SceneObject::setLocalScale(const glm::vec3& a_scale)
{
	m_localTransform[0] = glm::normalize(m_localTransform[0]) * a_scale.x;
	m_localTransform[1] = glm::normalize(m_localTransform[1]) * a_scale.y;
	m_localTransform[2] = glm::normalize(m_localTransform[2]) * a_scale.z;
	updateGlobalTransform();
}

void SceneObject::setLocalScale(float a_x, float a_y, float a_z)
{
	m_localTransform[0] = glm::normalize(m_localTransform[0]) * a_x;
	m_localTransform[1] = glm::normalize(m_localTransform[1]) * a_y;
	m_localTransform[2] = glm::normalize(m_localTransform[2]) * a_z;
	updateGlobalTransform();
}

glm::vec3 SceneObject::getLocalScale() const
{
	glm::vec3 v;
	v.x = glm::length(m_localTransform[0]);
	v.y = glm::length(m_localTransform[1]);
	v.z = glm::length(m_localTransform[2]);
	return v;
}

glm::vec3 SceneObject::getGlobalScale() const
{
	glm::vec3 v;
	v.x = glm::length(m_globalTransform[0]);
	v.y = glm::length(m_globalTransform[1]);
	v.z = glm::length(m_globalTransform[2]);
	return v;
}

// overrides local scale/rotation but keeps local translation
void SceneObject::setLocalRotation(const glm::quat& a_rotation)
{
	glm::vec4 t = m_localTransform[3];
	m_localTransform = glm::toMat4(a_rotation);
	m_localTransform[3] = t;
	updateGlobalTransform();
}

void SceneObject::setLocalRotation(const glm::mat3& a_transform)
{
	glm::vec4 t = m_localTransform[3];
	m_localTransform = glm::mat4(a_transform);
	m_localTransform[3] = t;
	updateGlobalTransform();
}

void SceneObject::setLocalRotation(const glm::mat4& a_transform)
{
	glm::vec4 t = m_localTransform[3];
	m_localTransform = a_transform;
	m_localTransform[3] = t;
	updateGlobalTransform();
}

// transforms entire local transform
void SceneObject::transformLocal(const glm::mat4& a_transform)
{
	m_localTransform = m_localTransform * a_transform;
	updateGlobalTransform();
}

void SceneObject::updateGlobalTransform()
{
	if (m_parent)
	{
		m_globalTransform = m_parent->getGlobalTransform() * m_localTransform;
	}
	else
	{
		m_globalTransform = m_localTransform;
	}
}

void SceneObject::detachController( SceneController* a_controller )
{
	auto iter = m_controllers.begin();
	while (iter < m_controllers.end())
	{
		if ( *iter == a_controller )
		{
			m_controllers.erase(iter);
			return;
		}
		++iter;
	}
}

void SceneObject::updateControllers( float a_deltaTime )
{
	for (auto& controller : m_controllers)
		controller->update( this, a_deltaTime );
}

} // namespace syn