#include <render/Material.h>
//#include <scene/Camera.h>
#include <GL/glew.h>

namespace syn
{

Material* Material::sm_boundMaterial = nullptr;

const char* Material::getTextureChannelName(TextureChannel a_channel)
{
	switch (a_channel)
	{
	case Diffuse:		return "diffuseMap";
	case Glow:			return "glowMap";
	case Ambient:		return "ambientMap";
	case Specular:		return "specularMap";
	case Gloss:			return "glossMap";
	case Normal:		return "normalMap";
	case Alpha:			return "alphaMap";
	case Environment:	return "environmentMap";
	case Displacement:	return "displacementMap";
	case Light:			return "lightMap";
	case Shadow:		return "shadowMap";
	default:			return nullptr;
	};

	return nullptr;
}

void Material::bind()
{
	Shader* shader = m_shader;

	if (shader == nullptr)
		shader = Shader::getBoundShader();

	if (sm_boundMaterial != this)
	{
		// bind material data
		shader->setUniform("material.ambient", m_ambient);
		shader->setUniform("material.diffuse", m_diffuse);
		shader->setUniform("material.specular", m_specular);
		shader->setUniform("material.emissive", m_emissive);

		// bind textures
		int channel = 0;
		for (auto& texture : m_textures)
		{
			if (texture != nullptr)
				texture->bind(channel);
			++channel;
		}	

		sm_boundMaterial = this;
	}

	shader->bind();
}

} // namespace syn