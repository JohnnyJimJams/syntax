#include <render/Texture.h>
#include <render/ResourceLibrary.h>
#include <GL/glew.h>

namespace syn 
{

Texture::~Texture()
{
	glDeleteTextures(1, &m_handle);
}

void Texture::release()
{
	ResourceLibrary::getSingleton()->releaseTexture(this);
}

void Texture::bind(int a_slot /* = -1 */)
{
	if (a_slot >= 0)
		glActiveTexture(GL_TEXTURE0 + a_slot);

	switch (m_type)
	{
	case Texture1D:		glBindTexture(GL_TEXTURE_1D, m_handle);			break;
	case Texture2D:		glBindTexture(GL_TEXTURE_2D, m_handle);			break;
	case Texture3D:		glBindTexture(GL_TEXTURE_3D, m_handle);			break;
	case TextureCube:	glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle);	break;
	default:	break;
	}
}

void Texture::setFilter(int a_filter)
{
	bind();
	switch (m_type)
	{
	case Texture1D:		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, a_filter);	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, a_filter);		break;
	case Texture2D:		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, a_filter);	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, a_filter);			break;
	case Texture3D:		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, a_filter);	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, a_filter);			break;
	case TextureCube:	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, a_filter);	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, a_filter);	break;
	default:	break;
	}
}

void Texture::setAddressing(int a_addressMode)
{
	bind();
	switch (m_type)
	{
	case Texture1D:		
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, a_addressMode);	
		break;
	case Texture2D:		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, a_addressMode);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, a_addressMode);			
		break;
	case Texture3D:		
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, a_addressMode);	
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, a_addressMode);	
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, a_addressMode);		
		break;
	case TextureCube:	
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, a_addressMode);	
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, a_addressMode);	
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, a_addressMode);
		break;
	default:	break;
	}
}

} // namespace syn