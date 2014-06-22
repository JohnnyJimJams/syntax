#pragma once

#include <scene/SceneObject.h>
#include <render/FrameBuffer.h>
#include <render/Shader.h>

namespace syn
{

class Shader;

DeclareSmartPtr(Light);

class SYNTAX_API Light : public SceneObject
{
	SynRTTI;

public:

	enum LightType : unsigned int
	{
		Directional	= 0,
		Point,
		Spot,
	};

	Light(LightType a_type = Directional)
		: m_type(a_type), m_ambient(0, 0, 0, 1), m_diffuse(1, 1, 1, 1), m_specular(1, 1, 1, 1), m_attenuation(1, 0, 0, 0), m_theta(0), m_phi(0), m_radius(1), m_hasShadow(false), m_shadowBuffer(nullptr) {}
	virtual ~Light() { destroyShadow(); }

	void				bind(Shader* a_shader, unsigned int a_index = 0);

	LightType			getLightType() const;

	glm::vec4&			getAmbient();
	glm::vec4&			getDiffuse();
	glm::vec4&			getSpecular();
	glm::vec4&			getAttenuation();
	float&				getDiffuseIntensity();
	float&				getSpecularIntensity();
	float&				getTheta();
	float&				getPhi();
	float&				getRadius();

	const glm::vec4&	getAmbient() const;
	const glm::vec4&	getDiffuse() const;
	const glm::vec4&	getSpecular() const;
	const glm::vec4&	getAttenuation() const;
	float				getDiffuseIntensity() const;
	float				getSpecularIntensity() const;
	float				getTheta() const;
	float				getPhi() const;
	float				getRadius() const;

	void				setAmbient(float a_red, float a_green, float a_blue);
	void				setDiffuse(float a_red, float a_green, float a_blue);
	void				setSpecular(float a_red, float a_green, float a_blue);
	void				setAmbient(const glm::vec3& a_ambient);
	void				setDiffuse(const glm::vec3& a_diffuse);
	void				setSpecular(const glm::vec3& a_specular);
	void				setAmbient(const glm::vec4& a_ambient);
	void				setDiffuse(const glm::vec4& a_diffuse);
	void				setSpecular(const glm::vec4& a_specular);
	void				setAttenuation(float a_constant, float a_linear, float a_quadratic);
	void				setAttenuation(const glm::vec3& a_attenuation);
	void				setAttenuation(const glm::vec4& a_attenuation);
	void				setDiffuseIntensity(float a_intensity);
	void				setSpecularIntensity(float a_intensity);
	void				setTheta(float a_theta);
	void				setPhi(float a_phi);
	void				setRadius(float a_radius);

	bool				hasShadow() const;
	void				setupShadow(unsigned int a_width, unsigned int a_height, float a_far, unsigned int a_textureWidth, unsigned int a_textureHeight, float a_bias = 0.001f);
	void				gatherShadows(SceneObject* a_scene);
	void				beginGatheringShadows();
	void				destroyShadow();
	const glm::mat4&	getShadowProjection() const;
	const glm::mat4&	getShadowMatrix() const;
	const glm::ivec2&	getShadowDimensions() const;
	Texture*			getShadowMap();
	float				getShadowFar();
	void				setShadowBias(float a_bias);
	float				getShadowBias();
	void				operator () (SceneObject* a_object);

	virtual void		updateGlobalTransform();
	
protected:

	LightType	m_type;

	glm::vec4	m_ambient;
	glm::vec4	m_diffuse;
	glm::vec4	m_specular;
	glm::vec4	m_attenuation;
	float		m_theta;
	float		m_phi;
	float		m_radius;

	// shadow data
	bool			m_hasShadow;
	float			m_shadowFar;
	glm::ivec2		m_shadowDimensions;
	glm::mat4		m_shadowMatrix;
	glm::mat4		m_shadowProjection;
	FrameBuffer*	m_shadowBuffer;
	float			m_shadowBias;

	static Shader*	sm_shadowShader;
};

inline Light::LightType Light::getLightType() const
{
	return m_type;
}

inline glm::vec4& Light::getAmbient()
{
	return m_ambient;
}

inline glm::vec4& Light::getDiffuse()
{
	return m_diffuse;
}

inline glm::vec4& Light::getSpecular()
{
	return m_specular;
}

inline glm::vec4& Light::getAttenuation()
{
	return m_attenuation;
}

inline float& Light::getDiffuseIntensity()
{
	return m_diffuse.a;
}

inline float& Light::getSpecularIntensity()
{
	return m_specular.a;
}

inline float& Light::getTheta()
{
	return m_theta;
}

inline float& Light::getPhi()
{
	return m_phi;
}

inline float& Light::getRadius()
{
	return m_radius;
}

inline const glm::vec4& Light::getAmbient() const
{
	return m_ambient;
}

inline const glm::vec4& Light::getDiffuse() const
{
	return m_diffuse;
}

inline const glm::vec4& Light::getSpecular() const
{
	return m_specular;
}

inline const glm::vec4& Light::getAttenuation() const
{
	return m_attenuation;
}

inline float Light::getDiffuseIntensity() const
{
	return m_diffuse.a;
}

inline float Light::getSpecularIntensity() const
{
	return m_specular.a;
}

inline float Light::getTheta() const
{
	return m_theta;
}

inline float Light::getPhi() const
{
	return m_phi;
}

inline float Light::getRadius() const
{
	return m_radius;
}

inline void Light::setAmbient(float a_red, float a_green, float a_blue)
{
	m_ambient.r = a_red;
	m_ambient.g = a_green;
	m_ambient.b = a_blue;
}

inline void Light::setDiffuse(float a_red, float a_green, float a_blue)
{
	m_diffuse.r = a_red;
	m_diffuse.g = a_green;
	m_diffuse.b = a_blue;
}

inline void Light::setSpecular(float a_red, float a_green, float a_blue)
{
	m_specular.r = a_red;
	m_specular.g = a_green;
	m_specular.b = a_blue;
}

inline void Light::setAmbient(const glm::vec3& a_ambient)
{
	m_ambient.rgb = a_ambient;
}

inline void Light::setDiffuse(const glm::vec3& a_diffuse)
{
	m_diffuse.rgb = a_diffuse;
}

inline void Light::setSpecular(const glm::vec3& a_specular)
{
	m_specular.rgb = a_specular;
}

inline void Light::setAmbient(const glm::vec4& a_ambient)
{
	m_ambient = a_ambient;
}

inline void Light::setDiffuse(const glm::vec4& a_diffuse)
{
	m_diffuse = a_diffuse;
}

inline void Light::setSpecular(const glm::vec4& a_specular)
{
	m_specular = a_specular;
}

inline void Light::setAttenuation(float a_constant, float a_linear, float a_quadratic)
{
	m_attenuation.x = a_constant;
	m_attenuation.y = a_linear;
	m_attenuation.z = a_quadratic;
}

inline void Light::setAttenuation(const glm::vec3& a_attenuation)
{
	m_attenuation.xyz = a_attenuation;
}

inline void Light::setAttenuation(const glm::vec4& a_attenuation)
{
	m_attenuation = a_attenuation;
}

inline void Light::setDiffuseIntensity(float a_intensity)
{
	m_diffuse.a = a_intensity;
}

inline void Light::setSpecularIntensity(float a_intensity)
{
	m_specular.a = a_intensity;
}

inline void Light::setTheta(float a_theta)
{
	m_theta = a_theta;
}

inline void Light::setPhi(float a_phi)
{
	m_phi = a_phi;
}

inline void Light::setRadius(float a_radius)
{
	m_radius = a_radius;
}

inline bool Light::hasShadow() const
{
	return m_hasShadow;
}

inline const glm::mat4& Light::getShadowProjection() const
{
	return m_shadowProjection;
}

inline const glm::mat4& Light::getShadowMatrix() const
{
	return m_shadowMatrix;
}

inline const glm::ivec2& Light::getShadowDimensions() const
{
	return m_shadowDimensions;
}

inline Texture* Light::getShadowMap()
{
	return m_shadowBuffer->getTexture(0);
}

inline float Light::getShadowFar()
{
	return m_shadowFar;
}

inline void Light::setShadowBias(float a_bias)
{
	m_shadowBias = a_bias;
}

inline float Light::getShadowBias()
{
	return m_shadowBias;
}

} // namespace syn