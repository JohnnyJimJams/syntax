#pragma once

#include <scene/Node.h>

namespace syn
{

struct Vertex
{
	enum Offsets : char
	{
		PositionOffset = 0,
		NormalOffset = PositionOffset + 16,
		TangentOffset = NormalOffset + 16,
		BiNormalOffset = TangentOffset + 16,
		ColourOffset = BiNormalOffset + 16,
		IndicesOffset = ColourOffset + 16,
		WeightsOffset = IndicesOffset + 16,
		TexCoordOffset = WeightsOffset + 16,
		TexCoord2Offset = TexCoordOffset + 8,
	};

	enum Attributes : unsigned int
	{
		Position	=	1,
		Normal		=	2,
		Tangent		=	4,
		BiNormal	=	8,
		Colour		=	16,
		Indices		=	32,
		Weights		=	64,
		TexCoord0	=	128,
		TexCoord1	=	256,

		Dynamic		= 512,

		NormalMapped = Normal | Tangent | BiNormal | TexCoord0,
	};

	glm::vec4	position;
	glm::vec4	normal;
	glm::vec4	tangent;
	glm::vec4	binormal;
	glm::vec4	colour;
	glm::ivec4	indices;
	glm::vec4	weights;
	glm::vec4	texcoord;

	bool operator == (const Vertex& a_rhs) const
	{
		// compare exactness!
		return memcmp(this,&a_rhs,sizeof(Vertex)) == 0;
	}

	bool operator < (const Vertex& a_rhs) const
	{
		return memcmp(this,&a_rhs,sizeof(Vertex)) > 0;
	}
};

DeclareSmartPtr(Geometry);

class SYNTAX_API Geometry : public RefObject
{
public:

	enum Topology : unsigned int
	{
		Points	= 0,
		Lines,
		LineStrip,
		Triangles,
		TriangleStrip,
		Patches
	};
	
	Geometry(Topology a_topology,
		unsigned int a_vertexCount, Vertex* a_vertices, unsigned int a_vertexAttributes, 
		unsigned int a_indexCount, unsigned int* a_indices, bool a_ownData = false);
	Geometry(Topology a_topology,
			 unsigned int a_vertexCount, const Vertex* a_vertices, unsigned int a_vertexAttributes, 
			 unsigned int a_indexCount, const unsigned int* a_indices);
	virtual ~Geometry();

	void			build();

	void			bind();

	void			render();

	Topology		getTopology() const;
	void			setTopology(Topology a_topology);

	unsigned int	getVertexAttributes() const;

	unsigned int	getVertexArrayObject() const;
	unsigned int	getVertexBufferObject() const;
	unsigned int	getIndexBufferObject() const;

	unsigned int	getVertexCount() const;
	unsigned int	getIndexCount() const;

	void			remapVertices(const Vertex* a_vertices);

	// create basic primitives
	static Geometry*	createBox(unsigned int a_vertexAttributes);
//	static Geometry*	createBoxInverted(unsigned int a_vertexAttributes);
	static Geometry*	createBoxSimple();
	static Geometry*	createBoxSimpleInverted();
	static Geometry*	createSphere(unsigned int a_vertexAttributes, unsigned int a_segments, unsigned int a_rings);
	static Geometry*	createSphereInverted(unsigned int a_vertexAttributes, unsigned int a_segments, unsigned int a_rings);
	static Geometry*	createPlane(unsigned int a_vertexAttributes, const glm::vec2& a_uvScale = glm::vec2(1,1));
	static Geometry*	createDisc(unsigned int a_vertexAttributes, unsigned int a_segments);
	static Geometry*	createFullscreenQuad();

private:

	void	setupVertexAttributes();

private:

	enum BufferType
	{
		BufferType_Vertex0	= 0,
		BufferType_Index,

		BufferType_Count,
	};

	Topology		m_topology;
	unsigned int	m_vertexAttributes;

	unsigned int	m_vertexArrayObject;
	unsigned int	m_bufferObjects[BufferType_Count];

	unsigned int	m_vertexCount;
	unsigned int	m_indexCount;
	Vertex*			m_vertices;
	unsigned int*	m_indices;
};

inline Geometry::Topology Geometry::getTopology() const
{
	return m_topology;
}

inline void Geometry::setTopology(Topology a_topology)
{
	m_topology = a_topology;
}

inline unsigned int Geometry::getVertexAttributes() const
{
	return m_vertexAttributes;
}

inline unsigned int Geometry::getVertexArrayObject() const
{
	return m_vertexArrayObject;
}

inline unsigned int Geometry::getVertexBufferObject() const
{
	return m_bufferObjects[BufferType_Vertex0];
}

inline unsigned int Geometry::getIndexBufferObject() const
{
	return m_bufferObjects[BufferType_Index];
}

inline unsigned int Geometry::getVertexCount() const
{
	return m_vertexCount;
}

inline unsigned int Geometry::getIndexCount() const
{
	return m_indexCount;
}

} // namespace syn