#include "pch.h"
#include "ParticleManager.h"
#include "ParticleSystem.h"

#include "Entities/Camera.h"
#include "Shaders/Shader.h"

#include "Engine/Core/Renderer/Renderer.h"
#include "Engine/Core/Renderer/Renderer2D.h"

namespace GraphX
{
	ParticleManager::ParticleManager()
		: m_ParticleShader(nullptr), m_Camera(nullptr), m_Index(0), m_PoolCap(0)
	{ }

	void ParticleManager::Initialize(const Ref<const Camera>& Camera)
	{
		GX_PROFILE_FUNCTION()

		GX_ENGINE_ASSERT(!IsInitialized(), "Particle Manager already initialised");

		m_Camera = Camera;

		m_ParticleShader = Renderer::GetShaderLibrary().Load("res/Shaders/ParticleShader.glsl", "Particle");
	}


	void ParticleManager::AddParticleSystem(const Ref<ParticleSystem>& System)
	{
		const std::string& name = System->GetName();
		GX_ENGINE_ASSERT(!Exists(name), "Particle System {0} already exists.", name);
		m_ParticleSystems.emplace(name, System);
	}

	Ref<ParticleSystem> ParticleManager::GetParticleSystem(const std::string& name)
	{
		GX_ENGINE_ASSERT(Exists(name), "Particle System {0} does not exist.", name);
		return m_ParticleSystems[name];
	}

	void ParticleManager::Update(float DeltaTime)
	{
		GX_PROFILE_FUNCTION()
		
		GX_ENGINE_ASSERT(IsInitialized(), "Particle Manager is not Initialised");

		if (m_Camera->IsRenderStateDirty())
		{
			m_ParticleShader->Bind();
			m_ParticleShader->SetUniformMat4f("u_Projection", m_Camera->GetProjectionMatrix());
		}
		
		const GM::Matrix4& ViewMatrix = m_Camera->GetViewMatrix();
		const GM::Vector3 CameraViewSpacePos(ViewMatrix(0, 3), ViewMatrix(1, 3), ViewMatrix(2, 3));
		for (const auto& System : m_ParticleSystems)
		{
			System.second->Update(DeltaTime, CameraViewSpacePos, m_Camera->IsRenderStateDirty());
		}
		
	}

	void ParticleManager::SpawnParticles(float DeltaTime)
	{
		GX_PROFILE_FUNCTION()

		for (const auto& System : m_ParticleSystems)
		{
			if(System.second->IsActive())
				System.second->SpawnParticles(DeltaTime);
		}
	}

	void ParticleManager::RenderParticles()
	{
		GX_PROFILE_FUNCTION()

		GX_ENGINE_ASSERT(IsInitialized(), "Particle Manager is not Initialised");

		Renderer2D::RenderParticles(m_ParticleSystems);
	}

	bool ParticleManager::Exists(const std::string& name) const
	{
		return m_ParticleSystems.find(name) != m_ParticleSystems.end();
	}

	bool ParticleManager::IsInitialized()
	{
		if (m_Camera && m_ParticleShader)
			return true;

		return false;
	}

	ParticleManager::~ParticleManager()
	{
		
	}
}