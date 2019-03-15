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
		for (unsigned int i = 0; i < m_Particles.size(); i++)
		{
			m_Particles[i].Update(DeltaTime);

			// Check if the life span of the particle has expired
			if (m_Particles[i].IsToBeDestroyed())
				m_Particles.erase(m_Particles.begin() + i);
		}
	}

	void ParticleManager::RenderParticles()
	{
		PreRender();

		// Render
		static SimpleRenderer renderer;
		const gm::Matrix4 View = m_Camera.GetViewMatrix();
		for (Particle particle : m_Particles)
		{
			particle.Enable(*m_ParticleShader, View);
			renderer.Draw(Quad::GetVerticesCount());
		}

		PostRender();
	}

	void ParticleManager::AddParticle(const Particle& particle)
	{
		m_Particles.push_back(particle);
	}

	void ParticleManager::PreRender()
	{
		m_ParticleShader->Bind();
		Particle::GetQuad().Enable();
		GLCall(glDepthMask(false));	// Don't render the particles to the depth buffer
		m_ParticleShader->SetUniformMat4f("u_View", m_Camera.GetViewMatrix());
		m_ParticleShader->SetUniformMat4f("u_Projection", m_Camera.GetPerspectiveProjectionMatrix());
	}

	void ParticleManager::PostRender()
	{
		GLCall(glDepthMask(true));
		Particle::GetQuad().Disable();
		m_ParticleShader->UnBind();
	}
}