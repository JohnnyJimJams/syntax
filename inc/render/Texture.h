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
	
	enum Format : unsigned int
	{
		Unknown		= 0,
		Red			= 0x1909,
		RedAlpha	= 0x190A,
		RGB			= 0x1907,
		RGBA		= 0x1908,
	};
	
	Type			getType() const;

	unsigned int	getID() const;
	const char*		getName() const;

	unsigned int	getHandle() const;

	int				getWidth() const;
	int				getHeight() const;

	int				getFormat() const;

	void			setParameter(unsigned int a_param, int a_value);
	void			setParameter(unsigned int a_param, float a_value);
	
	void			bind(int a_slot = -1);

	void			release();

private:

	Texture(unsigned int a_id, const char* a_name, Type a_type);
	Texture(unsigned int a_id, const char* a_name, unsigned int a_handle, Type a_type);
	virtual ~Texture();

	unsigned int	m_id;
	char*			m_name;
	unsigned int	m_handle;
	Type			m_type;

	int				m_width;
	int				m_height;
	int				m_format;
	unsigned char*	m_data;
};

inline Texture::Texture(unsigned int a_id, const char* a_name, Type a_type)
	:  m_id(a_id), 
	m_name(new char[strlen(a_name)+1]), 
	m_handle(0),
	m_type(a_type),
	m_width(0),
	m_height(0),
	m_format(0),
	m_data(nullptr)
{
	strcpy(m_name,a_name);
}

inline Texture::Texture(unsigned int a_id, const char* a_name, unsigned int a_handle, Type a_type)
	:  m_id(a_id), 
	m_name(new char[strlen(a_name)+1]), 
	m_handle(a_handle),
	m_type(a_type),
	m_width(0),
	m_height(0),
	m_format(0),
	m_data(nullptr)
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

inline int Texture::getWidth() const
{
	return m_width;
}

inline int Texture::getHeight() const
{
	return m_height;
}

inline int Texture::getFormat() const
{
	return m_format;
}

} // namespace syn