#pragma once

#include <RefObject.h>
#include <string.h>

namespace syn
{

DeclareSmartPtr(Texture);

class SYNTAX_API Texture : public RefObject
{
	friend class ResourceLibrary;

public:

	enum Type : unsigned int
	{
		Texture1D	= 0,
		Texture2D,
		Texture3D,
		TextureCube,
	};

	enum Channels : unsigned int
	{
		Unknown	= 0,
		Red,
		RedAlpha,
		RGB,
		RGBA,
	};
	
	void	bind(int a_slot = -1);

	void	release();

	unsigned int	getID() const;

	const char*		getName() const;

	unsigned int	getHandle() const;

	Type			getType() const;

	void			setFilter(int a_filter);
	void			setAddressing(int a_addressMode);

private:

	Texture(unsigned int a_id, const char* a_name, Type a_type);
	Texture(unsigned int a_id, const char* a_name, unsigned int a_handle, Type a_type);
	virtual ~Texture();

	unsigned int	m_id;
	char*			m_name;
	unsigned int	m_handle;
	Type			m_type;
};

inline Texture::Texture(unsigned int a_id, const char* a_name, Type a_type)
	:  m_id(a_id), 
	m_name(new char[strlen(a_name)+1]), 
	m_handle(0),
	m_type(a_type)
{
	strcpy(m_name,a_name);
}

inline Texture::Texture(unsigned int a_id, const char* a_name, unsigned int a_handle, Type a_type)
	:  m_id(a_id), 
	m_name(new char[strlen(a_name)+1]), 
	m_handle(a_handle),
	m_type(a_type)
{
	strcpy(m_name,a_name);
}

inline unsigned int Texture::getID() const
{
	return m_id;
}

inline const char* Texture::getName() const
{
	return m_name;
}

inline unsigned int Texture::getHandle() const
{
	return m_handle;
}

inline Texture::Type Texture::getType() const
{
	return m_type;
}

} // namespace syn