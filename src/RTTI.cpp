#include <RTTI.h>
#include <string.h>

namespace syn
{

RTTI::RTTI(const char* a_name, const RTTI* a_base)
	: m_name(new char[ strlen(a_name) + 1 ]),
	m_basetype(a_base)
{
	strcpy(m_name,a_name);
}

} // namespace syn