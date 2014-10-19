#include <render/FrameBuffer.h>
#include <render/ResourceLibrary.h>
#include <utilities/Log.h>
#include <string.h>
#include <gl_core_4_4.h>

namespace syn
{

FrameBuffer* FrameBuffer::sm_currentBound = nullptr;

FrameBuffer::FrameBuffer(const char* a_name, unsigned int a_id,	unsigned int a_width, unsigned int a_height,
	unsigned int a_targetCount, FrameBufferDesc* a_targetDesc, bool a_depth /* = true */, bool a_stencil /* = false */)
	: m_name(a_name),
	m_id(a_id),
	m_frameBufferObject(0),
	m_targetCount(a_targetCount),
	m_texture(a_targetCount),
	m_targets(new unsigned int[a_targetCount])
{
	// depth
	if (a_depth == true)
	{
		glGenRenderbuffers(1, &m_depth);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depth);
		if (a_stencil == true)
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, a_width, a_height);
		else
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, a_width, a_height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	// texture
	std::string texName = a_name;
	texName += "_texture";
	char buf[16];
	for (unsigned int i = 0; i < m_targetCount; ++i)
	{
		std::string name = texName;
		name += itoa(i, buf, 16);
		m_texture[i] = ResourceLibrary::getSingleton()->createTexture(name.c_str(), a_width, a_height,a_targetDesc[i].internalformat, a_targetDesc[i].externalformat, a_targetDesc[i].datatype);
	}

	// frame buffer
	glGenFramebuffers(1, &m_frameBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
	for (unsigned int i = 0; i < m_targetCount; ++i)
	{
		m_targets[i] = GL_COLOR_ATTACHMENT0 + i;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_texture[i]->getHandle(), 0);
	}

	if (a_depth == true)
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth);
	if (a_stencil == true)
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depth);

	glDrawBuffers(m_targetCount, m_targets);

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
}

void FrameBuffer::bind()
{
	if (sm_currentBound != this)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);

		sm_currentBound = this;
	}
}

void FrameBuffer::unBind()
{
	if (sm_currentBound == this)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		sm_currentBound = nullptr;
	}
}

} // namespace syn