#include <scene/Node.h>

namespace syn
{

SynRTTIDerived(Node,SceneObject);

void Node::build()
{
	for (auto& child : m_children)
	{
		child->build();
	}
}

void Node::update(float a_deltaTime)
{
	SceneObject::update(a_deltaTime);

	for (auto& child : m_children)
	{
		child->update(a_deltaTime);
	}
}

void Node::render()
{
	if (m_visible == true)
	{
		SceneObject::render();

		for (auto& child : m_children)
		{
			child->render();
		}
	}
}

void Node::attachChild(SceneObject* a_child)
{
	m_children.push_back( a_child );
	a_child->setParent( this );
	a_child->updateGlobalTransform();
}

void Node::detachChild(SceneObject* a_child)
{
	auto iter = m_children.begin();
	auto end = m_children.end();
	while (iter < end)
	{
		if (*iter == a_child)
		{
			(*iter)->setParent( nullptr );
			(*iter)->updateGlobalTransform();

			m_children.erase(iter);
			break;
		}
		++iter;
	}	
}

void Node::detachFromParent()
{
	if (m_parent)
		m_parent->detachChild(this);
}

void Node::updateGlobalTransform()
{
	SceneObject::updateGlobalTransform();

	for (auto& child : m_children)
	{
		child->updateGlobalTransform();
	}
}

Object* Node::getObjectByName(const char* a_name)
{
	if (strcmp(m_name,a_name) == 0)
		return this;

	for (auto& child : m_children)
	{
		Object* obj = child->getObjectByName(a_name);
		if (obj != nullptr)
			return obj;
	}
	return nullptr;
}

void Node::getAllObjectsByName(const char* a_name, std::vector<Object*>& a_objects)
{
	if (strcmp(m_name, a_name) == 0)
		a_objects.push_back(this);

	for (auto& child : m_children)
	{
		child->getAllObjectsByName(a_name,a_objects);
	}
}

} // namespace syn