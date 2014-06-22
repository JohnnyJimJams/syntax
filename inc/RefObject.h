#pragma once

#include <SmartPointer.h>

namespace syn
{

class SYNTAX_API RefObject
{
public:

	virtual ~RefObject();

	unsigned int		getObjectID() const;

	void				incrementReferences();
	void				decrementReferences();

	unsigned int		getReferenceCount() const;

	static unsigned int	getTotalObjects();

protected:

	RefObject();

	unsigned int		m_objectID;
	unsigned int		m_references;

	static unsigned int	sm_nextID;
	static unsigned int	sm_totalObjects;
};

inline RefObject::RefObject()
	: m_references(0)
{
	m_objectID = sm_nextID++;
	++sm_totalObjects;
}

inline RefObject::~RefObject()
{
	--sm_totalObjects;
}

inline unsigned int RefObject::getObjectID() const
{
	return m_objectID;
}

inline unsigned int RefObject::getTotalObjects()
{
	return sm_totalObjects;
}

inline void RefObject::incrementReferences()
{
	++m_references;
}

inline void RefObject::decrementReferences()
{
	if (--m_references == 0)
		delete this;
}

inline unsigned int RefObject::getReferenceCount() const
{
	return m_references;
}

} // namespace syn
