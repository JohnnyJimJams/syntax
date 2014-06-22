#include <render/FrameBuffer.h>
#include <render/ResourceLibrary.h>
#include <utilities/Log.h>
#include <string.h>
#include <GL/glew.h>

namespace syn
{

FrameBuffer* FrameBuffer::sm_currentBound = nullptr;

FrameBuffer::FrameBuffer(const char* a_name, unsigned int a_id, unsigned int a_width, unsigned int a_height, unsigned int a_targetCount,
						 bool a_depth /* = true */, unsigned int* a_bufferFormat /* = nullptr */, unsigned int* a_channels /* = nullptr */, unsigned int* a_bufferType /* = nullptr */) 
	: m_name(new char[strlen(a_name)+1]), 
	m_id(a_id),
	m_frameBufferObject(0),
	m_targetCount(a_targetCount),
	m_texture(a_targetCount),
	m_targets(new unsigned int[a_targetCount])
{
	strcpy(m_name,a_name);

	// depth
	if (a_depth == true)
	{
		glGenRenderbuffers(1, &m_depth);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, a_width, a_height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
	
	// texture
	std::string texName = a_name;
	texName += "_texture";
	char buf[16];
	for ( unsigned int i = 0 ; i < m_targetCount ; ++i )
	{
		std::string name = texName;
		name += itoa(i,buf,16);
		m_texture[i] = ResourceLibrary::getSingleton()->createTexture(name.c_str(),a_width,a_height,
			a_bufferFormat != nullptr ? *a_bufferFormat : GL_RGBA,a_channels != nullptr ? *a_channels : GL_RGBA,a_bufferType != nullptr ? *a_bufferType : GL_UNSIGNED_BYTE);
	}

	// frame buffer
	glGenFramebuffers(1, &m_frameBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
	for ( unsigned int i = 0 ; i < m_targetCount ; ++i )
	{
		m_targets[i] = GL_COLOR_ATTACHMENT0 + i;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_texture[i]->getHandle(), 0);
	}
	if (a_depth == true)
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		logError("Error creating frame buffer [%s]!\n", a_name);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
	unBind();
	for (auto texture : m_texture)
		ResourceLibrary::getSingleton()->releaseTexture(texture);
	delete[] m_name; 
}

void FrameBuffer::bind()
{
//	if (sm_currentBound != this)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
	//	glDrawBuffers(m_targetCount,m_targets);

		sm_currentBound = this;
	}
}

void FrameBuffer::unBind()
{
	//if (sm_currentBound == this)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//	const unsigned int targets[2] = {GL_COLOR_ATTACHMENT0,0};
	///	glDrawBuffers(1,targets);

		sm_currentBound = nullptr;
	}
}

} // namespace syn