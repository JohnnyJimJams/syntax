#include <Object.h>
#include <string.h>

namespace syn
{

SynRTTIBase(Object);

Object* Object::getObjectByName(const char* a_name)
{
	if (strcmp(m_name, a_name) == 0)
		return this;
	return nullptr;
}

void Object::getAllObjectsByName(const char* a_name, std::vector<Object*>& a_objects)
{
	if (strcmp(m_name, a_name) == 0)
		a_objects.push_back(this);
}

void Object::setName(const char* a_name)
{
	delete[] m_name;
	m_name = new char[strlen(a_name) + 1];
	strcpy(m_name, a_name);
}

} // namespace syn