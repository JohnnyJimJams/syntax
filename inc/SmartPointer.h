#pragma once

#include <Syntax.h>

namespace syn
{

// Macro, auto declares an object as being able to be have smart pointer
#define DeclareSmartPtr(classname) \
	class classname; \
	typedef syn::SmartPointer<classname> classname##Ptr

template <class T>
class SmartPointer
{
public:

	SmartPointer(T* a_object = nullptr);
	SmartPointer(const SmartPointer& a_rhs);
	~SmartPointer();

	operator T* () const;
	T& operator * () const;
	T* operator -> () const;

	SmartPointer& operator = (const SmartPointer& a_rhs);
	SmartPointer& operator = (T* a_object);

	bool operator == (T* a_object) const;
	bool operator != (T* a_object) const;
	bool operator == (const SmartPointer& a_rhs) const;
	bool operator != (const SmartPointer& a_rhs) const;

protected:

	T*	m_object;
};

template <class T>
SmartPointer<T>::SmartPointer (T* a_object)
{
	m_object = a_object;
	if (m_object != nullptr)
		m_object->incrementReferences();
}

template <class T>
SmartPointer<T>::SmartPointer (const SmartPointer& a_rhs)
{
	m_object = a_rhs.m_object;
	if (m_object != nullptr)
		m_object->incrementReferences();
}

template <class T>
SmartPointer<T>::~SmartPointer ()
{
	if (m_object != nullptr)
		m_object->decrementReferences();
}

template <class T>
SmartPointer<T>::operator T* () const
{
	return m_object;
}

template <class T>
T& SmartPointer<T>::operator* () const
{
	return *m_object;
}

template <class T>
T* SmartPointer<T>::operator-> () const
{
	return m_object;
}

template <class T>
SmartPointer<T>& SmartPointer<T>::operator = (const SmartPointer& a_rhs)
{
	if (m_object != a_rhs.m_object)
	{
		if (a_rhs.m_object != nullptr)
			a_rhs.m_object->incrementReferences();

		if (m_object != nullptr)
			m_object->decrementReferences();

		m_object = a_rhs.m_object;
	}
	return *this;
}

template <class T>
SmartPointer<T>& SmartPointer<T>::operator = (T* a_object)
{
	if (m_object != a_object)
	{
		if (a_object != nullptr)
			a_object->incrementReferences();

		if (m_object != nullptr)
			m_object->decrementReferences();

		m_object = a_object;
	}
	return *this;
}

template <class T>
bool SmartPointer<T>::operator == (T* a_object) const
{
	return (m_object == a_object);
}

template <class T>
bool SmartPointer<T>::operator != (T* a_object) const
{
	return (m_object != a_object);
}

template <class T>
bool SmartPointer<T>::operator == (const SmartPointer& a_rhs) const
{
	return (m_object == a_rhs.m_object);
}

template <class T>
bool SmartPointer<T>::operator != (const SmartPointer& a_rhs) const
{
	return (m_object != a_rhs.m_object);
}

} // namespace syn