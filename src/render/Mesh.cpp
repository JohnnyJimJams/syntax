#include <render/Mesh.h>
#include <render/Shader.h>

namespace syn
{

SynRTTIDerived(Mesh,SceneObject);

void Mesh::build()
{
	for (auto& geometry : m_geometry)
	{
		geometry->build();
	}
}

void Mesh::render()
{
	// TODO: get rid of need to use old setUniform() call for worldTransform
	if (m_material == nullptr)
	{
		Shader::getInvalidShader()->setUniform("worldTransform", m_globalTransform);
		Shader::getInvalidShader()->bind();
	}
	else
	{
		m_material->getShader()->setUniform("worldTransform", m_globalTransform);
		m_material->bind();
	}

	// draw each chunk
	for (auto& geometry : m_geometry)
	{
		geometry->bind();
		geometry->render();
	}
}

void Mesh::detachGeometry( Geometry* a_geometry )
{
	auto iter = m_geometry.begin();
	auto end = m_geometry.end();
	for (; iter < end ; ++iter)
	{
		if ( *iter == a_geometry )
		{
			m_geometry.erase(iter);
			return;
		}
	}
}

void Mesh::detachGeometryAt( unsigned int a_index )
{
	auto iter = m_geometry.begin();
	auto end = m_geometry.end();
	for ( unsigned int i = 0 ; iter < end ; ++iter, ++i)
	{
		if ( i == a_index )
		{
			m_geometry.erase(iter);
			return;
		}
	}
}

} // namespace syn