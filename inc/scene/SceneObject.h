#pragma once

#include <Object.h>
#include <scene/SceneController.h>
#include <glm/glm.hpp>

namespace syn
{

class Node;
class Renderer;

DeclareSmartPtr(SceneObject);

class SYNTAX_API SceneObject : public Object
{
	SynRTTI;

public:

	SceneObject();
	SceneObject(const char* a_name);
	virtual ~SceneObject();

	virtual void	build() {}

	virtual bool	isNode() const	{	return false;	}

	virtual void	update(float a_deltaTime);
	virtual void	render();

	bool			getVisible() const;
	void			setVisible(bool a_visible);

	glm::vec4		getLocalRight() const;
	glm::vec4		getLocalUp() const;
	glm::vec4		getLocalForward() const;

	glm::vec4		getGlobalRight() const;
	glm::vec4		getGlobalUp() const;
	glm::vec4		getGlobalForward() const;

	// local and global transform access
	glm::mat4&			getLocalTransform();
	const glm::mat4&	getLocalTransform() const;
	const glm::mat4&	getGlobalTransform() const;

	// local and global translation (position) access
	const glm::vec4&	getLocalTranslation() const;
	const glm::vec4&	getGlobalTranslation() const;

	// modify local translation
	void			setLocalTransform(const glm::mat4& a_transform);
	void			setLocalTranslation(const glm::vec3& a_translation);
	void			setLocalTranslation(const glm::vec4& a_translation);
	void			setLocalTranslation(float a_x, float a_y, float a_z);
	void			translateLocal(const glm::vec3& a_translation);
	void			translateLocal(const glm::vec4& a_translation);
	void			translateLocal(float a_x, float a_y, float a_z);

	// scale the node
	void			setLocalScale(float a_uniformScale);
	void			setLocalScale(const glm::vec3& a_scale);
	void			setLocalScale(float a_x, float a_y, float a_z);

	glm::vec3		getLocalScale() const;
	glm::vec3		getGlobalScale() const;

	// overrides local scale/rotation but keeps local translation
	void			setLocalRotation(const glm::quat& a_rotation);
	void			setLocalRotation(const glm::mat3& a_transform);
	void			setLocalRotation(const glm::mat4& a_transform);

	// transforms entire local transform
	void			transformLocal(const glm::mat4& a_transform);

	// updates current global transform based on parent and local
	virtual void	updateGlobalTransform();

	// controllers
	unsigned int		getControllerCount() const;
	SceneController*	getController( unsigned int a_index ) const;
	void				attachController( SceneController* a_controller );
	void				detachController( SceneController* a_controller );
	void				detachAllControllers();

	// update controllers attached to this object
	virtual void	updateControllers( float a_deltaTime );

	void			setParent(Node* a_node);
	Node*			getParent();

	// traversal
	template <class T>
	static void traverse(SceneObject* a_object, T& a_functor);

	template <class T>
	static bool traverseFirstStop(SceneObject* a_object, T& a_functor);

private:

	SceneObject(const SceneObject& a_rhs);
	SceneObject& operator = (const SceneObject& a_rhs);

protected:

	bool			m_visible;
	Node*			m_parent;

	glm::mat4		m_localTransform;
	glm::mat4		m_globalTransform;

	std::vector< SceneControllerPtr >	m_controllers;
};

template <class T>
void SceneObject::traverse(SceneObject* a_object, T& a_functor)
{
	a_functor(a_object);

	if (a_object->isNode())
	{
		Node* node = (Node*)a_object;
		for (auto& c : node->m_children)
		{
			traverse(c,a_functor);
		}
	}
}

template <class T>
bool SceneObject::traverseFirstStop(SceneObject* a_object, T& a_functor)
{
	if (a_functor(a_object) == false)
		return false;

	if (a_object->isNode())
	{
		Node* node = (Node*)this;
		for (auto& c : node->m_children)
		{
			if (traverseFirstStop(c,a_functor) == false)
				return false;
		}
	}

	return true;
}

inline void SceneObject::update(float a_deltaTime)
{
	updateControllers(a_deltaTime);
}

inline bool SceneObject::getVisible() const
{
	return m_visible ;
}

inline void SceneObject::setVisible(bool a_visible)
{
	m_visible = a_visible;
}

inline glm::mat4& SceneObject::getLocalTransform()
{
	return m_localTransform;
}

inline const glm::mat4& SceneObject::getLocalTransform() const
{
	return m_localTransform;
}

inline const glm::mat4& SceneObject::getGlobalTransform() const
{
	return m_globalTransform;
}

inline const glm::vec4& SceneObject::getLocalTranslation() const
{
	return m_localTransform[3];
}

inline const glm::vec4& SceneObject::getGlobalTranslation() const
{
	return m_globalTransform[3];
}

inline unsigned int SceneObject::getControllerCount() const
{
	return m_controllers.size();
}

inline SceneController* SceneObject::getController( unsigned int a_index ) const
{
	return m_controllers[a_index];
}

inline void SceneObject::attachController( SceneController* a_controller )
{
	m_controllers.push_back( a_controller );
}

inline void SceneObject::detachAllControllers()
{
	m_controllers.clear();
}

inline void SceneObject::setParent(Node* a_node)
{
	m_parent = a_node;
}

inline Node* SceneObject::getParent()
{
	return m_parent;
}

inline glm::vec4 SceneObject::getLocalRight() const
{
	return m_localTransform[0];
}

inline glm::vec4 SceneObject::getLocalUp() const
{
	return m_localTransform[1];
}

inline glm::vec4 SceneObject::getLocalForward() const
{
	return m_localTransform[2] * -1.f;
}

inline glm::vec4 SceneObject::getGlobalRight() const
{
	return m_globalTransform[0];
}

inline glm::vec4 SceneObject::getGlobalUp() const
{
	return m_globalTransform[1];
}

inline glm::vec4 SceneObject::getGlobalForward() const
{
	return m_globalTransform[2] * -1.f;
}

} // namespace syn