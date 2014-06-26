#pragma once

#include <scene/SceneObject.h>
#include <render/Material.h>
#include <render/Geometry.h>

namespace syn
{

DeclareSmartPtr(Mesh);

class SYNTAX_API Mesh : public SceneObject
{
	SynRTTI;

public:

	Mesh();
	Mesh(const char* a_name);
	virtual ~Mesh() {}

	virtual void	build();

	virtual void	render();

	void			setMaterial( Material* a_material );
	Material*		getMaterial() const;

	unsigned int	getGeometryCount() const;
	void			attachGeometry( Geometry* a_geometry );
	Geometry*		getGeometry( unsigned int a_index ) const;
	void			detachGeometry( Geometry* a_geometry );
	void			detachGeometryAt( unsigned int a_index );
	void			detachAllGeometry();

	void			setCastsShadow(bool a_cast);
	bool			getCastsShadow() const;

protected:

	MaterialPtr					m_material;
	std::vector< GeometryPtr >	m_geometry;
	bool						m_castsShadow;
};

inline Mesh::Mesh()
	: m_material(Material::getInvalidMaterial()),
	m_castsShadow(true)
{

}

inline Mesh::Mesh(const char* a_name) 
	: SceneObject(a_name),
	m_material(Material::getInvalidMaterial()),
	m_castsShadow(true)
{

}

inline void Mesh::setMaterial( Material* a_material )
{
	m_material = a_material;
}

inline Material* Mesh::getMaterial() const
{
	return m_material;
}

inline unsigned int Mesh::getGeometryCount() const
{
	return m_geometry.size();
}

inline void Mesh::attachGeometry( Geometry* a_geometry )
{
	m_geometry.push_back( a_geometry );
}

inline Geometry* Mesh::getGeometry( unsigned int a_index ) const
{
	return m_geometry[a_index];
}

inline void Mesh::detachAllGeometry()
{
	m_geometry.clear();
}

inline void Mesh::setCastsShadow(bool a_cast)
{
	m_castsShadow = a_cast;
}

inline bool Mesh::getCastsShadow() const
{
	return m_castsShadow;
}

} // namespace syn