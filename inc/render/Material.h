#pragma once

#include <render/Texture.h>
#include <render/Shader.h>
#include <string.h>
#include <glm/glm.hpp>

namespace syn
{
	
DeclareSmartPtr(Material);

class SYNTAX_API Material : public RefObject
{
	friend class ResourceLibrary;

public:

	enum TextureChannel : unsigned int
	{
		Diffuse	= 0,
		Glow,
		Ambient,
		Specular,
		Gloss,
		Normal,
		Alpha,
		Environment,
		Displacement,
		Light,
		Shadow,

		TextureChannel_Count,

		LightBuffer	=	TextureChannel_Count,
		AOBuffer,
		SinTanLookup,
	};

	static const char*	getTextureChannelName(TextureChannel a_channel);

	void				bind();

	const char*			getName() const;

	Shader*				getShader() const;

	glm::vec4&			getAmbient();
	glm::vec4&			getDiffuse();
	glm::vec4&			getSpecular();
	glm::vec4&			getEmissive();

	const glm::vec4&	getAmbient() const;
	const glm::vec4&	getDiffuse() const;
	const glm::vec4&	getSpecular() const;
	const glm::vec4&	getEmissive() const;

	Texture*			getTexture(TextureChannel a_channel) const;

	float				getAlpha() const;
	float				getSpecularPower() const;
	float				getAmbientFactor() const;
	float				getEmissiveFactor() const;

	float&				getAlpha();
	float&				getSpecularPower();

	void				setShader(Shader* a_shader);

	void				setAmbient(float a_red, float a_green, float a_blue);
	void				setDiffuse(float a_red, float a_green, float a_blue);
	void				setSpecular(float a_red, float a_green, float a_blue);
	void				setEmissive(float a_red, float a_green, float a_blue);

	void				setAmbient(const glm::vec3& a_ambient);
	void				setDiffuse(const glm::vec3& a_diffuse);
	void				setSpecular(const glm::vec3& a_diffuse);
	void				setEmissive(const glm::vec3& a_emissive);

	void				setAmbient(const glm::vec4& a_ambient);
	void				setDiffuse(const glm::vec4& a_diffuse);
	void				setSpecular(const glm::vec4& a_diffuse);
	void				setEmissive(const glm::vec4& a_emissive);

	void				setTexture(TextureChannel a_channel, Texture* a_texture);

	void				setAlpha(float a_alpha);
	void				setSpecularPower(float a_specularPower);
	void				setAmbientFactor(float a_factor);
	void				setEmissiveFactor(float a_factor);

	void				setRoughness(float a_roughness);
	void				setFresnelScale(float a_fresnel);
	float				getRoughness() const;
	float				getFresnelScale() const;
	float&				getRoughness();
	float&				getFresnelScale();
	glm::vec2&			getRoughnessUniform();
	const glm::vec2&	getRoughnessUniform() const;
	
	bool				isBound() const;

	static Material*	getBoundMaterial();	

private:

	Material(const char* a_name, unsigned int a_id);
	virtual ~Material() { delete[] m_name; }

	unsigned int	m_id;
	char*			m_name;
	glm::vec4		m_ambient;
	glm::vec4		m_diffuse;
	glm::vec4		m_specular;
	glm::vec4		m_emissive;

	glm::vec2		m_roughness;

	TexturePtr		m_textures[TextureChannel_Count];

	ShaderPtr		m_shader;

	static Material*	sm_boundMaterial;
};

inline Material::Material(const char* a_name, unsigned int a_id)
	: m_id(a_id), m_name(new char[strlen(a_name) + 1]),
	m_ambient(1, 1, 1, 1), m_diffuse(1, 1, 1, 1), m_specular(1, 1, 1, 1), m_emissive(0, 0, 0, 0), m_roughness(1, 0.001f),
	m_shader(Shader::getInvalidShader())
{
	strcpy(m_name, a_name);
	memset(m_textures, 0, sizeof(Texture*)*TextureChannel_Count);
}

inline const char* Material::getName() const
{
	return m_name;
}

inline Shader* Material::getShader() const
{
	return m_shader;
}

inline glm::vec4& Material::getAmbient()
{
	return m_ambient;
}

inline glm::vec4& Material::getDiffuse()
{
	return m_diffuse;
}

inline glm::vec4& Material::getSpecular()
{
	return m_specular;
}

inline glm::vec4& Material::getEmissive()
{
	return m_emissive;
}

inline const glm::vec4& Material::getAmbient() const
{
	return m_ambient;
}

inline const glm::vec4& Material::getDiffuse() const
{
	return m_diffuse;
}

inline const glm::vec4& Material::getSpecular() const
{
	return m_specular;
}

inline const glm::vec4& Material::getEmissive() const
{
	return m_emissive;
}

inline Texture* Material::getTexture(TextureChannel a_channel) const
{
	return m_textures[a_channel];
}

inline float Material::getAlpha() const
{
	return m_diffuse.a;
}

inline float Material::getSpecularPower() const
{
	return m_specular.a;
}

inline float Material::getAmbientFactor() const
{
	return m_ambient.a;
}

inline float Material::getEmissiveFactor() const
{
	return m_emissive.a;
}

inline float& Material::getAlpha()
{
	return m_diffuse.a;
}

inline float& Material::getSpecularPower()
{
	return m_specular.a;
}

inline void Material::setShader(Shader* a_shader)
{
	m_shader = a_shader;
}

inline void Material::setAmbient(float a_red, float a_green, float a_blue)
{
	m_ambient.r = a_red;
	m_ambient.g = a_green;
	m_ambient.b = a_blue;
}

inline void Material::setDiffuse(float a_red, float a_green, float a_blue)
{
	m_diffuse.r = a_red;
	m_diffuse.g = a_green;
	m_diffuse.b = a_blue;
}

inline void Material::setSpecular(float a_red, float a_green, float a_blue)
{
	m_specular.r = a_red;
	m_specular.g = a_green;
	m_specular.b = a_blue;
}

inline void Material::setEmissive(float a_red, float a_green, float a_blue)
{
	m_emissive.r = a_red;
	m_emissive.g = a_green;
	m_emissive.b = a_blue;
}

inline void Material::setAmbient(const glm::vec3& a_ambient)
{
	m_ambient.rgb = a_ambient;
}

inline void Material::setDiffuse(const glm::vec3& a_diffuse)
{
	m_diffuse.rgb = a_diffuse;
}

inline void Material::setSpecular(const glm::vec3& a_specular)
{
	m_specular.rgb = a_specular;
}

inline void Material::setEmissive(const glm::vec3& a_emissive)
{
	m_emissive.rgb = a_emissive;
}

inline void Material::setAmbient(const glm::vec4& a_ambient)
{
	m_ambient = a_ambient;
}

inline void Material::setDiffuse(const glm::vec4& a_diffuse)
{
	m_diffuse = a_diffuse;
}

inline void Material::setSpecular(const glm::vec4& a_specular)
{
	m_specular = a_specular;
}

inline void Material::setEmissive(const glm::vec4& a_emissive)
{
	m_emissive = a_emissive;
}

inline void Material::setTexture(TextureChannel a_channel, Texture* a_texture)
{
	m_textures[a_channel] = a_texture;
}

inline void Material::setAlpha(float a_alpha)
{
	m_diffuse.a = a_alpha;
}

inline void Material::setSpecularPower(float a_specularPower)
{
	m_specular.a = a_specularPower;
}

inline void Material::setAmbientFactor(float a_factor)
{
	m_ambient.a = a_factor;
}

inline void Material::setEmissiveFactor(float a_factor)
{
	m_emissive.a = a_factor;
}

inline void Material::setRoughness(float a_roughness)
{
	m_roughness.x = a_roughness;
}

inline void Material::setFresnelScale(float a_fresnel)
{
	m_roughness.y = a_fresnel;
}

inline float Material::getRoughness() const
{
	return m_roughness.x;
}

inline float Material::getFresnelScale() const
{
	return m_roughness.y;
}

inline float& Material::getRoughness()
{
	return m_roughness.x;
}

inline float& Material::getFresnelScale()
{
	return m_roughness.y;
}

inline glm::vec2& Material::getRoughnessUniform()
{
	return m_roughness;
}

inline const glm::vec2& Material::getRoughnessUniform() const
{
	return m_roughness;
}

inline Material* Material::getBoundMaterial()
{
	return sm_boundMaterial;
}

} // namespace syn