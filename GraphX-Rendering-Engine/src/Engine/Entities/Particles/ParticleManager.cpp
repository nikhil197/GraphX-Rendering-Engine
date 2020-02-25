#include "pch.h"
#include "ParticleManager.h"

#include "Entities/Camera.h"
#include "Shaders/Shader.h"

#include "Engine/Core/Renderer/Renderer.h"
#include "Engine/Core/Renderer/Renderer2D.h"

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

		m_ParticleShader = Renderer::GetShaderLibrary().Load("res/Shaders/ParticleShader.glsl", "Particle");
		if (m_Particles != nullptr)
		{
			m_Particles->resize(m_PoolCap);
		}
		else
		{
			m_Particles = CreateScope<std::vector<Particle>>(m_PoolCap);
		}
	}

	void ParticleManager::Update(float DeltaTime)
	{
		GX_PROFILE_FUNCTION()
		Check()

		if (m_Camera->IsRenderStateDirty())
		{
			m_ParticleShader->Bind();
			m_ParticleShader->SetUniformMat4f("u_Projection", m_Camera->GetProjectionMatrix());
		}
		
		const GM::Matrix4& ViewMatrix = m_Camera->GetViewMatrix();
		const GM::Vector3 CameraViewSpacePos(ViewMatrix(0, 3), ViewMatrix(1, 3), ViewMatrix(2, 3));
		for (unsigned int i = 0; i < m_PoolCap; i++)
		{
			m_Particles->at(i).Update(DeltaTime, CameraViewSpacePos, m_Camera->IsRenderStateDirty());
		}
	}

	void ParticleManager::RenderParticles()
	{
		GX_PROFILE_FUNCTION()

		Check()

		Renderer2D::RenderParticles(*m_Particles);
	}

	void ParticleManager::EmitParticle(const ParticleProps& props)
	{
		GX_PROFILE_FUNCTION()

		Check();

		m_Particles->at(m_Index).Init(props);
		m_Index = (m_Index + 1) % m_PoolCap;
	}

	bool ParticleManager::IsInitialized()
	{
		if (m_Camera && m_Particles && m_ParticleShader)
			return true;

		return false;
	}

	ParticleManager::~ParticleManager()
	{
		
	}
}