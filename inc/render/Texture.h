#pragma once

#include <RefObject.h>
#include <string>

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

protected:

	Texture(unsigned int a_id, const char* a_name, Type a_type);
	Texture(unsigned int a_id, const char* a_name, unsigned int a_handle, Type a_type);
	virtual ~Texture();

	unsigned int	m_id;
	std::string		m_name;
	unsigned int	m_handle;
	Type			m_type;

	int				m_width;
	int				m_height;
	int				m_format;
	unsigned char*	m_data;
};

class SYNTAX_API CubeTexture : public Texture
{
	friend class ResourceLibrary;

public:

protected:

	CubeTexture(unsigned int a_id, const char* a_name);
	CubeTexture(unsigned int a_id, const char* a_name, unsigned int a_handle);
	virtual ~CubeTexture();

	std::string		m_filenames[6];
	unsigned char*	m_extraData[5];
};

inline Texture::Texture(unsigned int a_id, const char* a_name, Type a_type)
	:  m_id(a_id), 
	m_name(a_name), 
	m_handle(0),
	m_type(a_type),
	m_width(0),
	m_height(0),
	m_format(0),
	m_data(nullptr)
{

}

inline Texture::Texture(unsigned int a_id, const char* a_name, unsigned int a_handle, Type a_type)
	:  m_id(a_id), 
	m_name(a_name), 
	m_handle(a_handle),
	m_type(a_type),
	m_width(0),
	m_height(0),
	m_format(0),
	m_data(nullptr)
{

}

inline unsigned int Texture::getID() const
{
	return m_id;
}

inline const char* Texture::getName() const
{
	return m_name.c_str();
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

inline CubeTexture::CubeTexture(unsigned int a_id, const char* a_name)
	: Texture(a_id,a_name,Texture::TextureCube)
{
	memset(m_extraData, 0, sizeof(unsigned char*) * 5);
}

inline CubeTexture::CubeTexture(unsigned int a_id, const char* a_name, unsigned int a_handle)
	: Texture(a_id,a_name,a_handle,Texture::TextureCube)
{
	memset(m_extraData, 0, sizeof(unsigned char*) * 5);
}

inline CubeTexture::~CubeTexture()
{
	delete[] m_extraData[0];
	delete[] m_extraData[1];
	delete[] m_extraData[2];
	delete[] m_extraData[3];
	delete[] m_extraData[4];
}

} // namespace syn