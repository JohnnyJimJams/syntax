#pragma once

#include <render/Shader.h>
#include <scene/Camera.h>
#include <render/Material.h>
#include <glm/glm.hpp>

namespace syn
{

class SYNTAX_API SharedFloatUniform : public Shader::SharedUniformBase
{
public:

	SharedFloatUniform(Shader::UniformBase* a_uniform, float a_value) : SharedUniformBase(a_uniform), m_value(a_value) {}
	virtual ~SharedFloatUniform() {}

	virtual void set()
	{
		Shader::Uniform<float>* uniform = (Shader::Uniform<float>*)m_uniform;
		uniform->set(m_value);
	}

	float	m_value;
};

class SYNTAX_API SharedFloatUniformFactory : public Shader::SharedUniformFactoryBase
{
public:

	SharedFloatUniformFactory(float a_value) : m_value(a_value) {}

	virtual ~SharedFloatUniformFactory() {}

	virtual Shader::SharedUniformBase* create(Shader::UniformBase* a_uniform)
	{
		return new SharedFloatUniform(a_uniform,m_value);
	}

	float	m_value;
};

class SYNTAX_API SharedFloatPtrUniform : public Shader::SharedUniformBase
{
public:

	SharedFloatPtrUniform(Shader::UniformBase* a_uniform, float* a_value) : SharedUniformBase(a_uniform), m_value(a_value) {}
	virtual ~SharedFloatPtrUniform() {}

	virtual void set()
	{
		Shader::Uniform<float>* uniform = (Shader::Uniform<float>*)m_uniform;
		uniform->set(*m_value);
	}

	float*	m_value;
};

class SYNTAX_API SharedFloatPtrUniformFactory : public Shader::SharedUniformFactoryBase
{
public:

	SharedFloatPtrUniformFactory(float* a_value) : m_value(a_value) {}

	virtual ~SharedFloatPtrUniformFactory() {}

	virtual Shader::SharedUniformBase* create(Shader::UniformBase* a_uniform)
	{
		return new SharedFloatPtrUniform(a_uniform,m_value);
	}

	float*	m_value;
};

class SYNTAX_API SharedIntUniform : public Shader::SharedUniformBase
{
public:

	SharedIntUniform(Shader::UniformBase* a_uniform, int a_index) : SharedUniformBase(a_uniform), m_index(a_index) {}
	virtual ~SharedIntUniform() {}

	virtual void set()
	{
		Shader::Uniform<int>* uniform = (Shader::Uniform<int>*)m_uniform;
		uniform->set(m_index);
	}

	int	m_index;
};

class SYNTAX_API SharedIntUniformFactory : public Shader::SharedUniformFactoryBase
{
public:

	SharedIntUniformFactory(int a_index) : m_index(a_index) {}

	virtual ~SharedIntUniformFactory() {}

	virtual Shader::SharedUniformBase* create(Shader::UniformBase* a_uniform)
	{
		return new SharedIntUniform(a_uniform,m_index);
	}

	int	m_index;
};

class SYNTAX_API SharedIntPtrUniform : public Shader::SharedUniformBase
{
public:

	SharedIntPtrUniform(Shader::UniformBase* a_uniform, int* a_index) : SharedUniformBase(a_uniform), m_index(a_index) {}
	virtual ~SharedIntPtrUniform() {}

	virtual void set()
	{
		Shader::Uniform<int>* uniform = (Shader::Uniform<int>*)m_uniform;
		uniform->set(*m_index);
	}

	int*	m_index;
};

class SYNTAX_API SharedIntPtrUniformFactory : public Shader::SharedUniformFactoryBase
{
public:

	SharedIntPtrUniformFactory(int* a_index) : m_index(a_index) {}

	virtual ~SharedIntPtrUniformFactory() {}

	virtual Shader::SharedUniformBase* create(Shader::UniformBase* a_uniform)
	{
		return new SharedIntPtrUniform(a_uniform,m_index);
	}

	int*	m_index;
};

class SYNTAX_API SharedIVec2Uniform : public Shader::SharedUniformBase
{
public:

	SharedIVec2Uniform(Shader::UniformBase* a_uniform, glm::ivec2* a_value) : SharedUniformBase(a_uniform), m_value(a_value) {}
	virtual ~SharedIVec2Uniform() {}

	virtual void set()
	{
		Shader::Uniform<glm::ivec2>* uniform = (Shader::Uniform<glm::ivec2>*)m_uniform;
		uniform->set(*m_value);
	}

	glm::ivec2*	m_value;
};

class SYNTAX_API SharedIVec2UniformFactory : public Shader::SharedUniformFactoryBase
{
public:

	SharedIVec2UniformFactory(glm::ivec2* a_value) : m_value(a_value) {}

	virtual ~SharedIVec2UniformFactory() {}

	virtual Shader::SharedUniformBase* create(Shader::UniformBase* a_uniform)
	{
		return new SharedIVec2Uniform(a_uniform,m_value);
	}

	glm::ivec2*		m_value;
};

class SYNTAX_API SharedVec4Uniform : public Shader::SharedUniformBase
{
public:

	SharedVec4Uniform(Shader::UniformBase* a_uniform, glm::vec4* a_value) : SharedUniformBase(a_uniform), m_value(a_value) {}
	virtual ~SharedVec4Uniform() {}

	virtual void set()
	{
		Shader::Uniform<glm::vec4>* uniform = (Shader::Uniform<glm::vec4>*)m_uniform;
		uniform->set(*m_value);
	}

	glm::vec4*	m_value;
};

class SYNTAX_API SharedVec4UniformFactory : public Shader::SharedUniformFactoryBase
{
public:

	SharedVec4UniformFactory(glm::vec4* a_value) : m_value(a_value) {}

	virtual ~SharedVec4UniformFactory() {}

	virtual Shader::SharedUniformBase* create(Shader::UniformBase* a_uniform)
	{
		return new SharedVec4Uniform(a_uniform,m_value);
	}

	glm::vec4*		m_value;
};

class SYNTAX_API SharedNearFarUniform : public Shader::SharedUniformBase
{
public:

	SharedNearFarUniform(Shader::UniformBase* a_uniform) : SharedUniformBase(a_uniform) {}
	virtual ~SharedNearFarUniform() {}

	virtual void set()
	{
		Shader::Uniform<glm::vec2>* uniform = (Shader::Uniform<glm::vec2>*)m_uniform;
		glm::vec2 nearFar;
		nearFar.x = Camera::getActiveCamera()->getNear();
		nearFar.y = Camera::getActiveCamera()->getFar();
		uniform->set(nearFar);
	}
};

class SYNTAX_API SharedCameraPositionUniform : public Shader::SharedUniformBase
{
public:

	SharedCameraPositionUniform(Shader::UniformBase* a_uniform) : SharedUniformBase(a_uniform) {}
	virtual ~SharedCameraPositionUniform() {}

	virtual void set()
	{
		Shader::Uniform<glm::vec4>* uniform = (Shader::Uniform<glm::vec4>*)m_uniform;
		uniform->set(Camera::getActiveCamera()->getGlobalTranslation());
	}
};

class SYNTAX_API SharedCameraViewUniform : public Shader::SharedUniformBase
{
public:

	SharedCameraViewUniform(Shader::UniformBase* a_uniform) : SharedUniformBase(a_uniform) {}
	virtual ~SharedCameraViewUniform() {}

	virtual void set()
	{
		Shader::Uniform<glm::mat4>* uniform = (Shader::Uniform<glm::mat4>*)m_uniform;
		uniform->set(Camera::getActiveCamera()->getView());
	}
};

class SYNTAX_API SharedCameraProjectionUniform : public Shader::SharedUniformBase
{
public:

	SharedCameraProjectionUniform(Shader::UniformBase* a_uniform) : SharedUniformBase(a_uniform) {}
	virtual ~SharedCameraProjectionUniform() {}

	virtual void set()
	{
		Shader::Uniform<glm::mat4>* uniform = (Shader::Uniform<glm::mat4>*)m_uniform;
		uniform->set(Camera::getActiveCamera()->getProjection());
	}
};

class SYNTAX_API SharedCameraProjectionViewUniform : public Shader::SharedUniformBase
{
public:

	SharedCameraProjectionViewUniform(Shader::UniformBase* a_uniform) : SharedUniformBase(a_uniform) {}
	virtual ~SharedCameraProjectionViewUniform() {}

	virtual void set()
	{
		Shader::Uniform<glm::mat4>* uniform = (Shader::Uniform<glm::mat4>*)m_uniform;
		uniform->set(Camera::getActiveCamera()->getProjectionView());
	}
};

class SYNTAX_API SharedCameraInvProjectionUniform : public Shader::SharedUniformBase
{
public:

	SharedCameraInvProjectionUniform(Shader::UniformBase* a_uniform) : SharedUniformBase(a_uniform) {}
	virtual ~SharedCameraInvProjectionUniform() {}

	virtual void set()
	{
		Shader::Uniform<glm::mat4>* uniform = (Shader::Uniform<glm::mat4>*)m_uniform;
		uniform->set(Camera::getActiveCamera()->getInverseProjection());
	}
};

class SYNTAX_API SharedCameraInvProjectionViewUniform : public Shader::SharedUniformBase
{
public:

	SharedCameraInvProjectionViewUniform(Shader::UniformBase* a_uniform) : SharedUniformBase(a_uniform) {}
	virtual ~SharedCameraInvProjectionViewUniform() {}

	virtual void set()
	{
		Shader::Uniform<glm::mat4>* uniform = (Shader::Uniform<glm::mat4>*)m_uniform;
		uniform->set(Camera::getActiveCamera()->getInverseProjectionView());
	}
};

class SYNTAX_API SharedCameraInvViewUniform : public Shader::SharedUniformBase
{
public:

	SharedCameraInvViewUniform(Shader::UniformBase* a_uniform) : SharedUniformBase(a_uniform) {}
	virtual ~SharedCameraInvViewUniform() {}

	virtual void set()
	{
		Shader::Uniform<glm::mat4>* uniform = (Shader::Uniform<glm::mat4>*)m_uniform;
		uniform->set(Camera::getActiveCamera()->getGlobalTransform());
	}
};

class SYNTAX_API SharedMaterialDiffuseUniform : public Shader::SharedUniformBase
{
public:

	SharedMaterialDiffuseUniform(Shader::UniformBase* a_uniform) : SharedUniformBase(a_uniform) {}
	virtual ~SharedMaterialDiffuseUniform() {}

	virtual void set()
	{
		Shader::Uniform<glm::vec4>* uniform = (Shader::Uniform<glm::vec4>*)m_uniform;
		uniform->set(Material::getBoundMaterial()->getDiffuse());
	}
};

class SYNTAX_API SharedMaterialAmbientUniform : public Shader::SharedUniformBase
{
public:

	SharedMaterialAmbientUniform(Shader::UniformBase* a_uniform) : SharedUniformBase(a_uniform) {}
	virtual ~SharedMaterialAmbientUniform() {}

	virtual void set()
	{
		Shader::Uniform<glm::vec4>* uniform = (Shader::Uniform<glm::vec4>*)m_uniform;
		uniform->set(Material::getBoundMaterial()->getAmbient());
	}
};

class SYNTAX_API SharedMaterialSpecularUniform : public Shader::SharedUniformBase
{
public:

	SharedMaterialSpecularUniform(Shader::UniformBase* a_uniform) : SharedUniformBase(a_uniform) {}
	virtual ~SharedMaterialSpecularUniform() {}

	virtual void set()
	{
		Shader::Uniform<glm::vec4>* uniform = (Shader::Uniform<glm::vec4>*)m_uniform;
		uniform->set(Material::getBoundMaterial()->getSpecular());
	}
};

class SYNTAX_API SharedMaterialEmissiveUniform : public Shader::SharedUniformBase
{
public:

	SharedMaterialEmissiveUniform(Shader::UniformBase* a_uniform) : SharedUniformBase(a_uniform) {}
	virtual ~SharedMaterialEmissiveUniform() {}

	virtual void set()
	{
		Shader::Uniform<glm::vec4>* uniform = (Shader::Uniform<glm::vec4>*)m_uniform;
		uniform->set(Material::getBoundMaterial()->getEmissive());
	}
};

} // namespace syn