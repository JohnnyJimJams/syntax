#pragma once

#include <scene/SceneObject.h>
#include <vector>

namespace syn
{

	DeclareSmartPtr(SceneNode);

class SYNTAX_API SceneNode : public SceneObject
{
	friend class SceneObject;

	SynRTTI;

public:

	typedef std::vector< SceneObjectPtr >	ChildArray;

	SceneNode();
	SceneNode(const char* a_name);
	virtual ~SceneNode();

	virtual void	build();

	virtual bool	isNode() const	{	return true;	}

	// update the node and its children
	virtual void	update(float a_deltaTime);

	// render the node and its children
	virtual void	render();

	// child management
	void			attachChild(SceneObject* a_child);
	void			detachChild(SceneObject* a_child);
	void			detachFromParent();

	unsigned int	getChildCount() const;
	SceneObject*	getChild(unsigned int a_index) const;

	// updates current global transform based on parent and local
	virtual void	updateGlobalTransform();

	virtual Object*	getObjectByName(const char* a_name);
	virtual void	getAllObjectsByName(const char* a_name, std::vector<Object*>& a_objects);

private:

	SceneNode(const SceneNode& a_rhs);
	SceneNode& operator = (const SceneNode& a_rhs);

protected:

	ChildArray	m_children;
};

inline SceneNode::SceneNode()
{

}

inline SceneNode::SceneNode(const char* a_name)
	: SceneObject(a_name)
{

}

inline SceneNode::~SceneNode()
{

}

inline unsigned int SceneNode::getChildCount() const
{
	return m_children.size();
}

inline SceneObject* SceneNode::getChild(unsigned int a_index) const
{
	return m_children[a_index];
}

} // namespace syn