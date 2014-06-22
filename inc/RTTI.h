#pragma once 

#include <Syntax.h>

namespace syn
{

class SYNTAX_API RTTI
{
public:

	RTTI(const char* a_name, const RTTI* a_base);
	~RTTI();

	const char*	getName() const;
	bool		isExactly(const RTTI& a_type) const;
	bool		isDerived(const RTTI& a_type) const;

private:

	char*		m_name;
	const RTTI*	m_basetype;
};

inline RTTI::~RTTI()
{
	delete[] m_name;
}

inline const char* RTTI::getName() const
{
	return m_name;
}

inline bool RTTI::isExactly(const RTTI& a_type) const
{
	return this == &a_type;
}

inline bool RTTI::isDerived(const RTTI& a_type) const
{
	const RTTI* search = this;
	while (search != nullptr)
	{
		if (search == &a_type)
			return true;
		search = search->m_basetype;
	}
	return false;
}

#define SynRTTI public: static const RTTI TYPE; virtual const RTTI& getType() const { return TYPE; }
#define SynRTTIBase(type) const RTTI type::TYPE(#type,nullptr)
#define SynRTTIDerived(type,basetype) const RTTI type::TYPE(#type,&(basetype::TYPE))

} // namespace syn