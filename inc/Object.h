#pragma once

#include <RefObject.h>
#include <RTTI.h>
#include <vector>

namespace syn
{

class SYNTAX_API Object : public RefObject
{
	SynRTTI;

public:

	virtual ~Object();

	void		setName(const char* a_name);
	const char*	getName() const;

	virtual Object*	getObjectByName(const char* a_name);
	virtual void	getAllObjectsByName(const char* a_name, std::vector<Object*>& a_objects);

	bool		isExactly(const RTTI& a_type) const;
	bool		isDerived(const RTTI& a_type) const;
	bool		isExactlyTypeOf(const Object* a_other) const;
	bool		isDerivedTypeOf(const Object* a_other) const;

protected:

	Object();

	char*	m_name;
};

inline Object::Object()
	: m_name(nullptr)
{

}

inline Object::~Object()
{
	delete[] m_name;
}

inline const char* Object::getName() const
{
	return m_name;
}

inline bool Object::isExactly(const RTTI& a_type) const
{
	return getType().isExactly(a_type);
}

inline bool Object::isDerived(const RTTI& a_type) const
{
	return getType().isDerived(a_type);
}

inline bool Object::isExactlyTypeOf(const Object* a_other) const
{
	return a_other != nullptr && getType().isExactly(a_other->getType());
}

inline bool Object::isDerivedTypeOf(const Object* a_other) const
{
	return a_other != nullptr && getType().isDerived(a_other->getType());
}

} // namespace syn

