#include "pch.h"
#include "ParticleManager.h"

#include "ErrorHandler.h"	// TODO: This should not be included outside of Core Submodule
#include "Entities/Camera.h"
#include "Model/Quad.h"
#include "Shaders/Shader.h"
#include "Core/Renderer/Renderer.h"

namespace GraphX
{
#define Check() {															\
		if(!IsInitialized())												\
		{																	\
			GX_ASSERT(false, "Particles Manager has not been Initialised");	\
		}																	\
	}

	ParticleManager::ParticleManager()
		: m_ParticleShader(nullptr), m_Particles(nullptr), m_Camera(nullptr), m_Index(0), m_PoolCap(0)
	{ }

	void ParticleManager::Initialize(const Camera* Camera, const int PoolCap)
	{
		m_Camera = Camera;
		m_PoolCap = PoolCap;

		m_ParticleShader = new Shader("res/Shaders/Particle.shader");
		if (m_Particles != nullptr)
		{
			m_Particles->resize(m_PoolCap);
		}
		else
		{
			m_Particles = new std::vector<Particle>(PoolCap);
		}
	}

	void ParticleManager::Update(float DeltaTime)
	{
		Check()

		const GraphXMaths::Matrix4& ViewMat = m_Camera->GetViewMatrix();
		for (unsigned int i = 0; i < m_PoolCap; i++)
		{
			m_Particles->at(i).Update(DeltaTime, ViewMat, m_Camera->IsRenderStateDirty());
		}
	}

	void ParticleManager::RenderParticles()
	{
		Check()

		PreRender();

		const GraphXMaths::Matrix4 View = m_Camera->GetViewMatrix();

		for (unsigned int i = 0; i < m_PoolCap; i++)
		{
			if (m_Particles->at(i).IsUsed())
			{
				m_Particles->at(i).Enable(*m_ParticleShader);
				Renderer::Render(Quad::GetVerticesCount());
			}
		}

		PostRender();
	}

	void ParticleManager::AddParticle(const GraphXMaths::Vector3& Position, const GraphXMaths::Vector3& Velocity, float LifeSpan, float Rotation, const class Texture* texture, float Scale, float GravityEffect)
	{
		Check();

		m_Particles->at(m_Index).Init(Position, Velocity, LifeSpan, Rotation, texture, Scale, GravityEffect);
		m_Index = (m_Index + 1) % m_PoolCap;
	}

	void ParticleManager::PreRender()
	{
		m_ParticleShader->Bind();
		Particle::GetQuad().Enable();
		GLCall(glDepthMask(false));	// Don't render the particles to the depth buffer
		m_ParticleShader->SetUniformMat4f("u_ProjectionView", m_Camera->GetProjectionViewMatrix());

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

	bool ParticleManager::IsInitialized()
	{
		if (m_Camera == nullptr)
			return false;
		if (m_Particles == nullptr)
			return false;
		if (m_ParticleShader == nullptr)
			return false;

		return true;
	}

	ParticleManager::~ParticleManager()
	{
		if (m_ParticleShader != nullptr)
		{
			delete m_ParticleShader;
			m_ParticleShader = nullptr;
		}

		if (m_Particles != nullptr)
		{
			delete m_Particles;
			m_Particles = nullptr;
		}
	}
}