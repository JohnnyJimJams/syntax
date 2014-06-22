#include <render/Light.h>
#include <render/Shader.h>
#include <render/ResourceLibrary.h>
#include <render/Mesh.h>
#include <scene/Camera.h>
#include <string>
#include <GL/glew.h>
#include <glm/ext.hpp>

namespace syn
{

SynRTTIDerived(Light,SceneObject);

Shader* Light::sm_shadowShader = nullptr;

void Light::bind(Shader* a_shader, unsigned int a_index /* = 0 */)
{
	char buffer[16];
	sprintf(buffer,"light[%i].",a_index);
	std::string light = buffer;

	a_shader->setUniform((light + "position").c_str(), m_globalTransform[3]);
	a_shader->setUniform((light + "direction").c_str(), -m_globalTransform[2]);
	a_shader->setUniform((light + "ambient").c_str(), m_ambient);
	a_shader->setUniform((light + "diffuse").c_str(), m_diffuse);
	a_shader->setUniform((light + "specular").c_str(), m_specular);
	a_shader->setUniform((light + "attenuation").c_str(), m_attenuation);
	a_shader->setUniform((light + "theta").c_str(), m_theta);
	a_shader->setUniform((light + "phi").c_str(), m_phi);
}

void Light::updateGlobalTransform()
{
	SceneObject::updateGlobalTransform();
}

void Light::setupShadow(unsigned int a_width, unsigned int a_height, float a_far, unsigned int a_textureWidth, unsigned int a_textureHeight, float a_bias /* = 0.001f */)
{
	m_shadowBias = a_bias;
	m_shadowDimensions.x = a_textureWidth;
	m_shadowDimensions.y = a_textureHeight;

	m_shadowFar = a_far;
	unsigned int format = GL_R32F;
	unsigned int channel = GL_RED;
	unsigned int type = GL_FLOAT;
	m_shadowBuffer = ResourceLibrary::getSingleton()->createFrameBuffer("shadowMap",a_textureWidth, a_textureHeight,1,true,&format,&channel,&type);
	m_shadowBuffer->getTexture(0)->bind(0);
	float colour[] = {1,1,1,1};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, colour);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	m_shadowProjection = glm::ortho<float>( -(a_width * 0.5f), a_width * 0.5f, -(a_height * 0.5f), a_height * 0.5f, 0.1f, m_shadowFar );

	m_shadowMatrix = m_shadowProjection * glm::inverse( m_globalTransform );

	m_hasShadow = true;

	if (sm_shadowShader == nullptr)
	{
		sm_shadowShader = ResourceLibrary::getSingleton()->getShader("directionalShadow");
	}
}

void Light::beginGatheringShadows()
{
	m_shadowBuffer->bind();
	glViewport(0,0,m_shadowDimensions.x,m_shadowDimensions.y);
	glClearColor(1,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	sm_shadowShader->setUniform("shadowProjection", m_shadowProjection);
	sm_shadowShader->setUniform("shadowFar", m_shadowFar);
}

void Light::gatherShadows(SceneObject* a_scene)
{
	// render geometry
	SceneObject::traverse(a_scene,*this);
	m_shadowBuffer->unBind();
}

void Light::operator () (SceneObject* a_object)
{
	if (a_object->isExactly(Mesh::TYPE))
	{
		Mesh* mesh = (Mesh*)a_object;
		if (mesh->getCastsShadow() == true)
		{
			sm_shadowShader->setUniform("shadowViewModel", glm::inverse( m_globalTransform ) * mesh->getGlobalTransform());
			sm_shadowShader->bind();

			unsigned int geometryCount = mesh->getGeometryCount();
			for ( unsigned int i = 0 ; i < geometryCount ; ++i )
			{
				Geometry* geometry = mesh->getGeometry(i);
				geometry->bind();
				geometry->render();
			}
		}
	}
}

void Light::destroyShadow()
{
	m_hasShadow = false;
	ResourceLibrary::getSingleton()->releaseFrameBuffer(m_shadowBuffer);
}

} // namespace syn