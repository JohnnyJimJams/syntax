#include <render/Geometry.h>
#include <GL/glew.h>
#include <glm/ext.hpp>

namespace syn
{

Geometry::Geometry(Topology a_topology,
	unsigned int a_vertexCount, const Vertex* a_vertices, unsigned int a_vertexAttributes, 
	unsigned int a_indexCount, const unsigned int* a_indices)
	: m_topology(a_topology),
	m_vertexAttributes(a_vertexAttributes),
	m_vertexCount(a_vertexCount),
	m_indexCount(a_indexCount),
	m_vertices(nullptr),
	m_indices(nullptr)
{
	m_bufferObjects[BufferType_Vertex0] = 0;
	m_bufferObjects[BufferType_Index] = 0;

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	if (m_vertexCount > 0)
	{
		glGenBuffers(1, &m_bufferObjects[BufferType_Vertex0]);
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[BufferType_Vertex0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertexCount, a_vertices, (m_vertexAttributes & Vertex::Dynamic) == 0 ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	}

	if (m_indexCount > 0)
	{
		glGenBuffers(1, &m_bufferObjects[BufferType_Index]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferObjects[BufferType_Index]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indexCount, a_indices, GL_STATIC_DRAW);
	}

	setupVertexAttributes();

	glBindVertexArray(0);
}

Geometry::Geometry(Topology a_topology,
	unsigned int a_vertexCount, Vertex* a_vertices, unsigned int a_vertexAttributes, 
	unsigned int a_indexCount, unsigned int* a_indices, bool a_ownData /* = false */)
	: m_topology(a_topology),
	m_vertexAttributes(a_vertexAttributes),
	m_vertexCount(a_vertexCount),
	m_indexCount(a_indexCount),
	m_vertices(a_ownData ? a_vertices : nullptr),
	m_indices(a_ownData ? a_indices : nullptr)
{
	m_bufferObjects[BufferType_Vertex0] = 0;
	m_bufferObjects[BufferType_Index] = 0;

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	if (m_vertexCount > 0)
	{
		glGenBuffers(1, &m_bufferObjects[BufferType_Vertex0]);
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[BufferType_Vertex0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertexCount, a_vertices, (m_vertexAttributes & Vertex::Dynamic) == 0 ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	}

	if (m_indexCount > 0)
	{
		glGenBuffers(1, &m_bufferObjects[BufferType_Index]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferObjects[BufferType_Index]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indexCount, a_indices, GL_STATIC_DRAW);
	}

	setupVertexAttributes();

	glBindVertexArray(0);
}

Geometry::~Geometry()
{
	delete[] m_vertices;
	delete[] m_indices;
	glDeleteBuffers(BufferType_Count, m_bufferObjects);
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void Geometry::build()
{
	if (m_vertices == nullptr)
		return;

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	if (m_vertexCount > 0)
	{
		glGenBuffers(1, &m_bufferObjects[BufferType_Vertex0]);
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[BufferType_Vertex0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertexCount, m_vertices, (m_vertexAttributes & Vertex::Dynamic) == 0 ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	}

	if (m_indexCount > 0)
	{
		glGenBuffers(1, &m_bufferObjects[BufferType_Index]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferObjects[BufferType_Index]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indexCount, m_indices, GL_STATIC_DRAW);
	}

	setupVertexAttributes();

	glBindVertexArray(0);
}

void Geometry::setupVertexAttributes()
{
	// setup attributes
	unsigned int attributeIndex = 0;

	if ((m_vertexAttributes & Vertex::Position) != 0)
	{
		glEnableVertexAttribArray(attributeIndex);
		glVertexAttribPointer(attributeIndex, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)Vertex::PositionOffset);
		++attributeIndex;
	}
	if ((m_vertexAttributes & Vertex::Normal) != 0)
	{
		glEnableVertexAttribArray(attributeIndex);
		glVertexAttribPointer(attributeIndex, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (char*)Vertex::NormalOffset);
		++attributeIndex;
	}
	if ((m_vertexAttributes & Vertex::Tangent) != 0)
	{
		glEnableVertexAttribArray(attributeIndex);
		glVertexAttribPointer(attributeIndex, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (char*)Vertex::TangentOffset);
		++attributeIndex;
	}
	if ((m_vertexAttributes & Vertex::BiNormal) != 0)
	{
		glEnableVertexAttribArray(attributeIndex);
		glVertexAttribPointer(attributeIndex, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (char*)Vertex::BiNormalOffset);
		++attributeIndex;
	}
	if ((m_vertexAttributes & Vertex::Colour) != 0)
	{
		glEnableVertexAttribArray(attributeIndex);
		glVertexAttribPointer(attributeIndex, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)Vertex::ColourOffset);
		++attributeIndex;
	}
	if ((m_vertexAttributes & Vertex::Indices) != 0)
	{
		glEnableVertexAttribArray(attributeIndex);
		glVertexAttribPointer(attributeIndex, 4, GL_INT, GL_FALSE, sizeof(Vertex), (char*)Vertex::IndicesOffset);
		++attributeIndex;
	}
	if ((m_vertexAttributes & Vertex::Weights) != 0)
	{
		glEnableVertexAttribArray(attributeIndex);
		glVertexAttribPointer(attributeIndex, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)Vertex::WeightsOffset);
		++attributeIndex;
	}
	if ((m_vertexAttributes & Vertex::TexCoord0) != 0)
	{
		glEnableVertexAttribArray(attributeIndex);
		glVertexAttribPointer(attributeIndex, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)Vertex::TexCoordOffset);
		++attributeIndex;
	}
	if ((m_vertexAttributes & Vertex::TexCoord1) != 0)
	{
		glEnableVertexAttribArray(attributeIndex);
		glVertexAttribPointer(attributeIndex, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)Vertex::TexCoord2Offset);
		++attributeIndex;
	}
}

void Geometry::bind()
{
	glBindVertexArray(m_vertexArrayObject);
}

void Geometry::render()
{
	if (m_indexCount > 0)
	{
		switch (m_topology)
		{
		case Points:			glDrawElements(GL_POINTS, m_indexCount, GL_UNSIGNED_INT, 0);		break;
		case Lines:				glDrawElements(GL_LINES, m_indexCount, GL_UNSIGNED_INT, 0);			break;
		case LineStrip:			glDrawElements(GL_LINE_STRIP, m_indexCount, GL_UNSIGNED_INT, 0);	break;
		case Triangles:			glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);		break;
		case TriangleStrip:		glDrawElements(GL_TRIANGLE_STRIP, m_indexCount, GL_UNSIGNED_INT, 0);break;
		case Patches:			glDrawElements(GL_PATCHES, m_indexCount, GL_UNSIGNED_INT, 0);		break;
		default:	break;
		};
	}
	else
	{
		switch (m_topology)
		{
		case Points:			glDrawArrays(GL_POINTS, 0, m_vertexCount);			break;
		case Lines:				glDrawArrays(GL_LINES, 0, m_vertexCount);			break;
		case LineStrip:			glDrawArrays(GL_LINE_STRIP, 0, m_vertexCount);		break;
		case Triangles:			glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);		break;
		case TriangleStrip:		glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vertexCount);	break;
		case Patches:			glDrawArrays(GL_PATCHES, 0, m_vertexCount);			break;
		default:	break;
		};
	}
}

void Geometry::remapVertices(const Vertex* a_vertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[BufferType_Vertex0]);
	Vertex* vertices = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(vertices, a_vertices, sizeof(Vertex) * m_vertexCount);
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

Geometry* Geometry::createBox(unsigned int a_vertexAttributes)
{
	// each face is unique
	Vertex vertices[4*6];
	unsigned int indices[6*6];

	// top
	vertices[0].position = glm::vec4(-.5f,.5f,-.5f,1);
	vertices[1].position = glm::vec4(.5f, .5f, -.5f, 1);
	vertices[2].position = glm::vec4(.5f, .5f, .5f, 1);
	vertices[3].position = glm::vec4(-.5f, .5f, .5f, 1);
	if ((a_vertexAttributes & Vertex::Normal) != 0)
	{
		vertices[0].normal = glm::vec4(0, 1, 0, 0);
		vertices[1].normal = glm::vec4(0, 1, 0, 0);
		vertices[2].normal = glm::vec4(0, 1, 0, 0);
		vertices[3].normal = glm::vec4(0, 1, 0, 0);
	}
	if ((a_vertexAttributes & Vertex::Tangent) != 0)
	{
		vertices[0].tangent = glm::vec4(1, 0, 0, 0);
		vertices[1].tangent = glm::vec4(1, 0, 0, 0);
		vertices[2].tangent = glm::vec4(1, 0, 0, 0);
		vertices[3].tangent = glm::vec4(1, 0, 0, 0);
	}
	if ((a_vertexAttributes & Vertex::BiNormal) != 0)
	{
		vertices[0].binormal = glm::vec4(0, 0, -1, 0);
		vertices[1].binormal = glm::vec4(0, 0, -1, 0);
		vertices[2].binormal = glm::vec4(0, 0, -1, 0);
		vertices[3].binormal = glm::vec4(0, 0, -1, 0);
	}
	if ((a_vertexAttributes & Vertex::TexCoord0) != 0)
	{
		vertices[0].texcoord.xy = glm::vec2(0, 1);
		vertices[1].texcoord.xy = glm::vec2(1, 1);
		vertices[2].texcoord.xy = glm::vec2(1, 0);
		vertices[3].texcoord.xy = glm::vec2(0, 0);
	}
	if ((a_vertexAttributes & Vertex::TexCoord1) != 0)
	{
		vertices[0].texcoord.zw = glm::vec2(0, 1);
		vertices[1].texcoord.zw = glm::vec2(1, 1);
		vertices[2].texcoord.zw = glm::vec2(1, 0);
		vertices[3].texcoord.zw = glm::vec2(0, 0);
	}
	// bottom
	vertices[4].position = glm::vec4(-.5f, -.5f, .5f, 1);
	vertices[5].position = glm::vec4(.5f, -.5f, .5f, 1);
	vertices[6].position = glm::vec4(.5f, -.5f, -.5f, 1);
	vertices[7].position = glm::vec4(-.5f, -.5f, -.5f, 1);
	if ((a_vertexAttributes & Vertex::Normal) != 0)
	{
		vertices[4].normal = glm::vec4(0, -1, 0, 0);
		vertices[5].normal = glm::vec4(0, -1, 0, 0);
		vertices[6].normal = glm::vec4(0, -1, 0, 0);
		vertices[7].normal = glm::vec4(0, -1, 0, 0);
	}
	if ((a_vertexAttributes & Vertex::Tangent) != 0)
	{
		vertices[4].tangent = glm::vec4(1, 0, 0, 0);
		vertices[5].tangent = glm::vec4(1, 0, 0, 0);
		vertices[6].tangent = glm::vec4(1, 0, 0, 0);
		vertices[7].tangent = glm::vec4(1, 0, 0, 0);
	}
	if ((a_vertexAttributes & Vertex::BiNormal) != 0)
	{
		vertices[4].binormal = glm::vec4(0, 0, 1, 0);
		vertices[5].binormal = glm::vec4(0, 0, 1, 0);
		vertices[6].binormal = glm::vec4(0, 0, 1, 0);
		vertices[7].binormal = glm::vec4(0, 0, 1, 0);
	}
	if ((a_vertexAttributes & Vertex::TexCoord0) != 0)
	{
		vertices[4].texcoord.xy = glm::vec2(0, 1);
		vertices[5].texcoord.xy = glm::vec2(1, 1);
		vertices[6].texcoord.xy = glm::vec2(1, 0);
		vertices[7].texcoord.xy = glm::vec2(0, 0);
	}
	if ((a_vertexAttributes & Vertex::TexCoord1) != 0)
	{
		vertices[4].texcoord.zw = glm::vec2(0, 1);
		vertices[5].texcoord.zw = glm::vec2(1, 1);
		vertices[6].texcoord.zw = glm::vec2(1, 0);
		vertices[7].texcoord.zw = glm::vec2(0, 0);
	}
	// left
	// right
	// back
	// front

	return new Geometry(Geometry::Triangles,4*6,vertices,Vertex::Position|a_vertexAttributes,6*6,indices);
}

Geometry* Geometry::createBoxInverted(unsigned int a_vertexAttributes)
{
	return nullptr;
}

Geometry* Geometry::createBoxSimple()
{
	Vertex vertices[8];
	vertices[0].position = glm::vec4(-0.5f, 0.5f, -0.5f, 1);
	vertices[1].position = glm::vec4(-0.5f, 0.5f, 0.5f, 1);
	vertices[2].position = glm::vec4(0.5f, 0.5f, 0.5f, 1);
	vertices[3].position = glm::vec4(0.5f, 0.5f, -0.5f, 1);
	vertices[4].position = glm::vec4(-0.5f, -0.5f, -0.5f, 1);
	vertices[5].position = glm::vec4(-0.5f, -0.5f, 0.5f, 1);
	vertices[6].position = glm::vec4(0.5f, -0.5f, 0.5f, 1);
	vertices[7].position = glm::vec4(0.5f, -0.5f, -0.5f, 1);

	unsigned int indices[] = {
		0,1,2,
		0,2,3,
		1,5,6,
		1,6,2,
		2,6,7,
		2,7,3,

		0,4,5,
		0,5,1,
		3,7,4,
		3,4,0,
		7,6,5,
		7,5,4,
	};

	return new Geometry(Geometry::Triangles,8,vertices,Vertex::Position,36,indices);
}

Geometry* Geometry::createBoxSimpleInverted()
{
	Vertex vertices[8];
	vertices[0].position = glm::vec4(-0.5f, 0.5f, -0.5f, 1);
	vertices[1].position = glm::vec4(-0.5f, 0.5f, 0.5f, 1);
	vertices[2].position = glm::vec4(0.5f, 0.5f, 0.5f, 1);
	vertices[3].position = glm::vec4(0.5f, 0.5f, -0.5f, 1);
	vertices[4].position = glm::vec4(-0.5f, -0.5f, -0.5f, 1);
	vertices[5].position = glm::vec4(-0.5f, -0.5f, 0.5f, 1);
	vertices[6].position = glm::vec4(0.5f, -0.5f, 0.5f, 1);
	vertices[7].position = glm::vec4(0.5f, -0.5f, -0.5f, 1);

	unsigned int indices[] = {
		2,1,0,
		3,2,0,
		6,5,1,
		2,6,1,
		7,6,2,
		3,7,2,

		5,4,0,
		1,5,0,
		4,7,3,
		0,4,3,
		5,6,7,
		4,5,7,
	};

	return new Geometry(Geometry::Triangles,8,vertices,Vertex::Position,36,indices);
}

Geometry* Geometry::createSphere(unsigned int a_vertexAttributes, unsigned int a_segments, unsigned int a_rings)
{
	unsigned int vertCount = (a_segments + 1) * (a_rings + 2);
	unsigned int indexCount = a_segments * (a_rings + 1) * 6;

	Vertex* vertices = new Vertex[ vertCount ]; 
	unsigned int* indices = new unsigned int[ indexCount ];

	float ringAngle = glm::pi<float>() / (a_rings + 1);
	float segmentAngle = 2.0f * glm::pi<float>() / a_segments;

	Vertex* vertex = vertices;

	for ( unsigned int ring = 0; ring < (a_rings + 2) ; ++ring )
	{
		float r0 = glm::sin(ring * ringAngle);
		float y0 = glm::cos(ring * ringAngle);

		for ( unsigned int segment = 0; segment < (a_segments + 1) ; ++segment, ++vertex )
		{
			float x0 = r0 * glm::sin(segment * segmentAngle);
			float z0 = r0 * glm::cos(segment * segmentAngle);

			vertex->position = glm::vec4(x0 * 0.5f, y0 * 0.5f, z0 * 0.5f, 1);

			if ((Vertex::Normal & a_vertexAttributes) != 0)
			{
				vertex->normal = glm::vec4(x0, y0, z0, 0);
			}

			if ((Vertex::Tangent & a_vertexAttributes) != 0)
			{
				vertex->tangent = glm::vec4(glm::sin(segment * segmentAngle + glm::half_pi<float>()), 0, glm::cos(segment * segmentAngle + glm::half_pi<float>()), 0);
			}

			if ((Vertex::BiNormal & a_vertexAttributes) != 0)
			{
				vertex->binormal = glm::vec4(glm::cross(vertex->normal.xyz(), vertex->tangent.xyz()), 0);
			}

			if ((Vertex::TexCoord0 & a_vertexAttributes) != 0)
			{
				vertex->texcoord.xy = glm::vec2(segment / (float)a_segments, 1 - ring / (float)(a_rings + 1));
			}

			if ((Vertex::TexCoord1 & a_vertexAttributes) != 0)
			{
				vertex->texcoord.zw = glm::vec2(segment / (float)a_segments, 1 - ring / (float)(a_rings + 1));
			}
		}
	}

	unsigned int index = 0;
	for(unsigned i = 0; i < (a_rings + 1); ++i)
	{
		for(unsigned j = 0; j < a_segments; ++j)
		{
			indices[index++] = i * (a_segments+1) + j;
			indices[index++] = (i+1) * (a_segments+1) + j;
			indices[index++] = i * (a_segments+1) + (j+1);

			indices[index++] = (i+1) * (a_segments+1) + (j+1);
			indices[index++] = i * (a_segments+1) + (j+1);
			indices[index++] = (i+1) * (a_segments+1) + j;
		}
	}
	
	Geometry* geometry = new Geometry(Geometry::Triangles,vertCount,vertices,Vertex::Position | a_vertexAttributes,indexCount,indices);

	delete[] indices;
	delete[] vertices;

	return geometry;
}

Geometry* Geometry::createSphereInverted(unsigned int a_vertexAttributes, unsigned int a_segments, unsigned int a_rings)
{
	unsigned int vertCount = (a_segments + 1) * (a_rings + 2);
	unsigned int indexCount = a_segments * (a_rings + 1) * 6;

	Vertex* vertices = new Vertex[ vertCount ]; 
	unsigned int* indices = new unsigned int[ indexCount ];

	float ringAngle = glm::pi<float>() / (a_rings + 1);
	float segmentAngle = 2.0f * glm::pi<float>() / a_segments;

	Vertex* vertex = vertices;

	for ( unsigned int ring = 0; ring < (a_rings + 2) ; ++ring )
	{
		float r0 = glm::sin(ring * ringAngle);
		float y0 = glm::cos(ring * ringAngle);

		for ( unsigned int segment = 0; segment < (a_segments + 1) ; ++segment, ++vertex )
		{
			float x0 = r0 * glm::sin(segment * segmentAngle);
			float z0 = r0 * glm::cos(segment * segmentAngle);

			vertex->position = glm::vec4(x0 * 0.5f, y0 * 0.5f, z0 * 0.5f, 1);

			if ((Vertex::Normal & a_vertexAttributes) != 0)
			{
				vertex->normal = glm::vec4(x0, y0, z0, 0);
			}

			if ((Vertex::Tangent & a_vertexAttributes) != 0)
			{
				vertex->tangent = glm::vec4(glm::sin(segment * segmentAngle + glm::half_pi<float>()), 0, glm::cos(segment * segmentAngle + glm::half_pi<float>()), 0);
			}

			if ((Vertex::BiNormal & a_vertexAttributes) != 0)
			{
				vertex->binormal = glm::vec4(glm::cross(vertex->normal.xyz(), vertex->tangent.xyz()), 0);
			}

			if ((Vertex::TexCoord0 & a_vertexAttributes) != 0)
			{
				vertex->texcoord.xy = glm::vec2(segment / (float)a_segments, 1 - ring / (float)(a_rings + 1));
			}

			if ((Vertex::TexCoord1 & a_vertexAttributes) != 0)
			{
				vertex->texcoord.zw = glm::vec2(segment / (float)a_segments, 1 - ring / (float)(a_rings + 1));
			}
		}
	}

	unsigned int index = 0;
	for(unsigned i = 0; i < (a_rings + 1); ++i)
	{
		for(unsigned j = 0; j < a_segments; ++j)
		{
			indices[index++] = i * (a_segments+1) + (j+1);
			indices[index++] = (i+1) * (a_segments+1) + j;
			indices[index++] = i * (a_segments+1) + j;

			indices[index++] = (i+1) * (a_segments+1) + j;
			indices[index++] = i * (a_segments+1) + (j+1);
			indices[index++] = (i+1) * (a_segments+1) + (j+1);
		}
	}

	Geometry* geometry = new Geometry(Geometry::Triangles,vertCount,vertices,Vertex::Position | a_vertexAttributes,indexCount,indices);

	delete[] indices;
	delete[] vertices;

	return geometry;
}

Geometry* Geometry::createPlane(unsigned int a_vertexAttributes, const glm::vec2& a_textureScale /* = glm::vec2(1,1) */)
{
	Vertex vertices[4];
	vertices[0].position = glm::vec4(-0.5f, 0, -0.5f, 1);
	vertices[1].position = glm::vec4(-0.5f, 0, 0.5f, 1);
	vertices[2].position = glm::vec4(0.5f, 0, -0.5f, 1);
	vertices[3].position = glm::vec4(0.5f, 0, 0.5f, 1);

	if ((a_vertexAttributes & Vertex::Normal) != 0)
	{
		vertices[0].normal = glm::vec4(0, 1, 0, 0);
		vertices[1].normal = glm::vec4(0, 1, 0, 0);
		vertices[2].normal = glm::vec4(0, 1, 0, 0);
		vertices[3].normal = glm::vec4(0, 1, 0, 0);
	}
	if ((a_vertexAttributes & Vertex::Tangent) != 0)
	{
		vertices[0].tangent = glm::vec4(1, 0, 0, 0);
		vertices[1].tangent = glm::vec4(1, 0, 0, 0);
		vertices[2].tangent = glm::vec4(1, 0, 0, 0);
		vertices[3].tangent = glm::vec4(1, 0, 0, 0);
	}
	if ((a_vertexAttributes & Vertex::BiNormal) != 0)
	{
		vertices[0].binormal = glm::vec4(0, 0, 1, 0);
		vertices[1].binormal = glm::vec4(0, 0, 1, 0);
		vertices[2].binormal = glm::vec4(0, 0, 1, 0);
		vertices[3].binormal = glm::vec4(0, 0, 1, 0);
	}
	if ((a_vertexAttributes & Vertex::TexCoord0) != 0)
	{
		vertices[0].texcoord.xy = glm::vec2(0, 0);
		vertices[1].texcoord.xy = glm::vec2(0, a_textureScale.y);
		vertices[2].texcoord.xy = glm::vec2(a_textureScale.x, 0);
		vertices[3].texcoord.xy = a_textureScale;
	}
	if ((a_vertexAttributes & Vertex::TexCoord1) != 0)
	{
		vertices[0].texcoord.zw = glm::vec2(0, 0);
		vertices[1].texcoord.zw = glm::vec2(0, a_textureScale.y);
		vertices[2].texcoord.zw = glm::vec2(a_textureScale.x, 0);
		vertices[3].texcoord.zw = a_textureScale;
	}

	return new Geometry(Geometry::TriangleStrip,4,vertices,Vertex::Position | a_vertexAttributes,0,nullptr);
}

Geometry* Geometry::createDisc(unsigned int a_vertexAttributes, unsigned int a_segments)
{
	unsigned int vertCount = a_segments + 1;
	unsigned int indexCount = a_segments * 3;

	Vertex* vertices = new Vertex[ vertCount ];
	unsigned int* indices = new unsigned int[ indexCount ];

	float segmentAngle = glm::pi<float>() * 2 / a_segments;

	// first vertex
	vertices[0].position = glm::vec4(0, 0, 0, 1);
	if ((a_vertexAttributes & Vertex::Normal) != 0)
	{
		vertices[0].normal = glm::vec4(0, 1, 0, 0);
	}
	if ((a_vertexAttributes & Vertex::Tangent) != 0)
	{
		vertices[0].tangent = glm::vec4(1, 0, 0, 0);
	}
	if ((a_vertexAttributes & Vertex::BiNormal) != 0)
	{
		vertices[0].binormal = glm::vec4(0, 0, 1, 0);
	}
	if ((a_vertexAttributes & Vertex::TexCoord0) != 0)
	{
		vertices[0].texcoord.xy = glm::vec2(0.5f, 0.5f);
	}
	if ((a_vertexAttributes & Vertex::TexCoord1) != 0)
	{
		vertices[0].texcoord.zw = glm::vec2(0.5f, 0.5f);
	}

	unsigned int index = 0;
	for ( unsigned int i = 0 ; i < a_segments ; ++i )
	{
		float s = glm::sin<float>(i * segmentAngle);
		float c = glm::cos<float>(i * segmentAngle);

		vertices[i + 1].position = glm::vec4(s * 0.5f, 0, c * 0.5f, 1);
		if ((a_vertexAttributes & Vertex::Normal) != 0)
		{
			vertices[i + 1].normal = glm::vec4(0, 1, 0, 0);
		}
		if ((a_vertexAttributes & Vertex::Tangent) != 0)
		{
			vertices[i + 1].tangent = glm::vec4(1, 0, 0, 0);
		}
		if ((a_vertexAttributes & Vertex::BiNormal) != 0)
		{
			vertices[i + 1].binormal = glm::vec4(0, 0, -1, 0);
		}
		if ((a_vertexAttributes & Vertex::TexCoord0) != 0)
		{
			vertices[i + 1].texcoord.xy = glm::vec2(s * 0.5f + 0.5f, 1 - (c * 0.5f + 0.5f));
		}
		if ((a_vertexAttributes & Vertex::TexCoord1) != 0)
		{
			vertices[i + 1].texcoord.zw = glm::vec2(s * 0.5f + 0.5f, 1 - (c * 0.5f + 0.5f));
		}

		indices[index++] = 0;
		indices[index++] = i + 1;
		indices[index++] = i + 2;
	}

	// correct last triangle
	indices[a_segments * 3 - 1] = 1;

	Geometry* geometry = new Geometry(Geometry::Triangles,vertCount,vertices,Vertex::Position | a_vertexAttributes,indexCount,indices);

	delete[] indices;
	delete[] vertices;

	return geometry;
}

Geometry* Geometry::createFullscreenQuad()
{
	Vertex vertices[4];
	vertices[0].position = glm::vec4(-1, -1, 1, 1);
	vertices[1].position = glm::vec4(1, -1, 1, 1);
	vertices[2].position = glm::vec4(-1, 1, 1, 1);
	vertices[3].position = glm::vec4(1, 1, 1, 1);
	return new Geometry(Geometry::TriangleStrip,4,vertices,Vertex::Position,0,nullptr);
}

} // namespace syn