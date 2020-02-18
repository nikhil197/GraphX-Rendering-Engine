#include "pch.h"
#include "ParticleManager.h"

#include "GL/glew.h"	// TODO: This should not be included outside of Core Submodule
#include "Entities/Camera.h"
#include "Model/Quad.h"
#include "Shaders/Shader.h"
#include "Core/Renderer/Renderer.h"

namespace GraphX
{
#define Check() {															\
		if(!IsInitialized())												\
		{																	\
			GX_ENGINE_ASSERT(false, "Particles Manager has not been Initialised");	\
		}																	\
	}

	ParticleManager::ParticleManager()
		: m_ParticleShader(nullptr), m_Particles(nullptr), m_Camera(nullptr), m_Index(0), m_PoolCap(0)
	{ }

	void ParticleManager::Initialize(const Ref<const Camera>& Camera, const int PoolCap)
	{
		GX_PROFILE_FUNCTION()

		m_Camera = Camera;
		m_PoolCap = PoolCap;

		m_ParticleShader = CreateRef<Shader>("res/Shaders/ParticleShader.glsl");
		if (m_Particles != nullptr)
		{
			m_Particles->resize(m_PoolCap);
		}
		else
		{
			m_Particles = CreateScope<std::vector<Particle>>(PoolCap);
		}
	}

	void ParticleManager::Update(float DeltaTime)
	{
		GX_PROFILE_FUNCTION()
		Check()

		const GM::Matrix4& ViewMat = m_Camera->GetViewMatrix();
		for (unsigned int i = 0; i < m_PoolCap; i++)
		{
			m_Particles->at(i).Update(DeltaTime, ViewMat, m_Camera->IsRenderStateDirty());
		}
	}

	void ParticleManager::RenderParticles()
	{
		GX_PROFILE_FUNCTION()

		Check()

		PreRender();

		const GM::Matrix4 View = m_Camera->GetViewMatrix();

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

	void ParticleManager::AddParticle(const GM::Vector3& Position, const GM::Vector3& Velocity, float LifeSpan, float Rotation, const Ref<Texture2D>& texture, float Scale, float GravityEffect)
	{
		GX_PROFILE_FUNCTION()

		Check();

		m_Particles->at(m_Index).Init(Position, Velocity, LifeSpan, Rotation, texture, Scale, GravityEffect);
		m_Index = (m_Index + 1) % m_PoolCap;
	}

	void ParticleManager::PreRender()
	{
		GX_PROFILE_FUNCTION()

		m_ParticleShader->Bind();
		Particle::GetQuad().Enable();
		glDepthMask(false);	// Don't render the particles to the depth buffer
		m_ParticleShader->SetUniformMat4f("u_ProjectionView", m_Camera->GetProjectionViewMatrix());

		// To enable blending
		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}

	void ParticleManager::PostRender()
	{
		GX_PROFILE_FUNCTION()

		glDepthMask(true);
		Particle::GetQuad().Disable();
		m_ParticleShader->UnBind();

		glDisable(GL_BLEND);
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
		
	}
}