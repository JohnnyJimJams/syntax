#include <render/Material.h>
//#include <scene/Camera.h>
#include <GL/glew.h>

namespace syn
{

Material* Material::sm_boundMaterial = nullptr;
Material* Material::sm_invalidMaterial = nullptr;

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
	if (sm_boundMaterial != this)
	{
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

	m_shader->bind();
}

} // namespace syn