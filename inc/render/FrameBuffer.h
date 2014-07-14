#pragma once

#include <render/Texture.h>
#include <vector>

namespace syn
{

struct SYNTAX_API FrameBufferDesc
{
	unsigned int	internalformat;
	unsigned int	externalformat;
	unsigned int	datatype;
};

DeclareSmartPtr(FrameBuffer);

class SYNTAX_API FrameBuffer : public RefObject
{
	friend class ResourceLibrary;

public:

	unsigned int	getID() const;
	const char*		getName() const;

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
		FrameBufferDesc* a_targetDesc,
		bool a_depth = true,
		bool a_stencil = false);
	virtual ~FrameBuffer();

	std::string				m_name;
	unsigned int			m_id;

	unsigned int			m_frameBufferObject;

	unsigned int			m_targetCount;
	std::vector<Texture*>	m_texture;
	unsigned int			m_depth;
	unsigned int*			m_targets;

	static FrameBuffer*	sm_currentBound;
};

inline unsigned int FrameBuffer::getID() const
{
	return m_id;
}

inline const char* FrameBuffer::getName() const
{
	return m_name.c_str();
}

} // namespace syn