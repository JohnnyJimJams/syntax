#include <render/Texture.h>
#include <render/ResourceLibrary.h>
#include <gl_core_4_4.h>

namespace syn 
{

Texture::~Texture()
{
	delete[] m_data;
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

void Texture::setParameter(unsigned int a_param, int a_value)
{
	bind();
	switch (m_type)
	{
	case Texture1D:		glTexParameteri(GL_TEXTURE_1D, a_param, a_value);		break;
	case Texture2D:		glTexParameteri(GL_TEXTURE_2D, a_param, a_value);		break;
	case Texture3D:		glTexParameteri(GL_TEXTURE_3D, a_param, a_value);		break;
	case TextureCube:	glTexParameteri(GL_TEXTURE_CUBE_MAP, a_param, a_value);	break;
	default:	break;
	}
}

void Texture::setParameter(unsigned int a_param, float a_value)
{
	bind();
	switch (m_type)
	{
	case Texture1D:		glTexParameterf(GL_TEXTURE_1D, a_param, a_value);		break;
	case Texture2D:		glTexParameterf(GL_TEXTURE_2D, a_param, a_value);		break;
	case Texture3D:		glTexParameterf(GL_TEXTURE_3D, a_param, a_value);		break;
	case TextureCube:	glTexParameterf(GL_TEXTURE_CUBE_MAP, a_param, a_value);	break;
	default:	break;
	}
}

} // namespace syn