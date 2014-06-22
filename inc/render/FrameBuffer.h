#pragma once

#include <render/Texture.h>
#include <vector>

namespace syn
{

DeclareSmartPtr(FrameBuffer);

class SYNTAX_API FrameBuffer : public RefObject
{
	friend class ResourceLibrary;

public:

	void		bind();
	void		unBind();

	Texture*	getTexture(unsigned int a_index) const	{	return m_texture[a_index];	}

	unsigned int*	getTargets() const { return m_targets; }

private:

	FrameBuffer(const char* a_name, 
		unsigned int a_id, 
		unsigned int a_width,
		unsigned int a_height,
		unsigned int a_targetCount, 
		bool a_depth = true,
		unsigned int* a_bufferFormat = nullptr,
		unsigned int* a_channels = nullptr,
		unsigned int* a_bufferType = nullptr);
	virtual ~FrameBuffer();

	char*			m_name;
	unsigned int	m_id;

	unsigned int	m_frameBufferObject;

	unsigned int			m_targetCount;
	std::vector<Texture*>	m_texture;
	unsigned int			m_depth;
	unsigned int*			m_targets;

	static FrameBuffer*	sm_currentBound;
};

} // namespace syn