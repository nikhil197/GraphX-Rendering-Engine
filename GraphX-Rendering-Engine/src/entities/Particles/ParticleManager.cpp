#include "pch.h"
#include "ParticleManager.h"

#include "Entities/Camera.h"
#include "Model/Quad.h"
#include "Shaders/Shader.h"
#include "Renderer/SimpleRenderer.h"

namespace engine
{
	ParticleManager::ParticleManager(const Camera& camera)
		: m_ParticleShader(new Shader("res/Shaders/Particle.shader")), m_Camera(camera)
	{
	}

	void ParticleManager::Update(float DeltaTime)
	{
		ParticlesMap::iterator itr = m_Particle.begin();
		for (; itr != m_Particle.end(); itr++)
		{
			std::vector<Particle>& particles = itr->second;
			for (unsigned int i = 0; i < particles.size(); i++)
			{
				particles[i].Update(DeltaTime, m_Camera.GetViewMatrix(), m_Camera.IsRenderStateDirty());

				// Check if the life span of the particle has expired
				if (particles[i].IsToBeDestroyed())
					particles.erase(particles.begin() + i);
			}
		}
	}

	void ParticleManager::RenderParticles()
	{
		PreRender();

		// Render
		static SimpleRenderer renderer;
		const gm::Matrix4 View = m_Camera.GetViewMatrix();

		ParticlesMap::iterator itr = m_Particle.begin();
		for (; itr != m_Particle.end(); itr++)
		{
			Texture& Tex = itr->first;
			Tex.Bind();
			m_ParticleShader->SetUniform1i("u_ParticleTexture", 0);
			m_ParticleShader->SetUniform1i("u_TexAtlasRows", (int)Tex.GetRowsInTexAtlas());

			std::vector<Particle>& particles = itr->second;
			for (Particle particle : particles)
			{
				particle.Enable(*m_ParticleShader);
				renderer.Draw(Quad::GetVerticesCount());
			}
		}

		PostRender();
	}

	void ParticleManager::AddParticle(const Particle& particle)
	{
		m_Particle[(Texture&)particle.GetTexture()].push_back(particle);
//		m_Particles.push_back(particle);
	}

	void ParticleManager::PreRender()
	{
		m_ParticleShader->Bind();
		Particle::GetQuad().Enable();
		GLCall(glDepthMask(false));	// Don't render the particles to the depth buffer
		m_ParticleShader->SetUniformMat4f("u_View", m_Camera.GetViewMatrix());
		m_ParticleShader->SetUniformMat4f("u_Projection", m_Camera.GetPerspectiveProjectionMatrix());

		// To enable blending
		GLCall(glEnable(GL_BLEND));

		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE));
	}

	void ParticleManager::PostRender()
	{
		GLCall(glDepthMask(true));
		Particle::GetQuad().Disable();
		m_ParticleShader->UnBind();

		GLCall(glDisable(GL_BLEND));
	}
}