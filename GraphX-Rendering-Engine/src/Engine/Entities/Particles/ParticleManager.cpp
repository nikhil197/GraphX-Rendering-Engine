#include "pch.h"
#include "ParticleManager.h"
#include "ParticleSystem.h"

#include "Entities/Camera.h"
#include "Shaders/Shader.h"

#include "Engine/Core/Renderer/Renderer.h"
#include "Engine/Core/Renderer/Renderer2D.h"

namespace GraphX
{
	ParticleManager::ParticleManagerData* ParticleManager::s_Data = nullptr;

	void ParticleManager::Init(const Ref<const Camera>& Camera)
	{
		GX_PROFILE_FUNCTION()

		GX_ENGINE_ASSERT(s_Data == nullptr, "Particle Manager already initialised");

		s_Data = new ParticleManager::ParticleManagerData();
		s_Data->Camera = Camera;
	}

	void ParticleManager::Shutdown()
	{
		GX_ENGINE_ASSERT(s_Data != nullptr, "Particle Manager already shutdown");
		delete s_Data;
	}

	void ParticleManager::AddParticleSystem(const Ref<ParticleSystem>& System)
	{
		const std::string& name = System->GetName();
		GX_ENGINE_ASSERT(!Exists(name), "Particle System {0} already exists.", name);
		s_Data->ParticleSystems.emplace(name, System);
	}

	Ref<ParticleSystem> ParticleManager::GetParticleSystem(const std::string& name)
	{
		GX_ENGINE_ASSERT(Exists(name), "Particle System {0} does not exist.", name);
		return s_Data->ParticleSystems[name];
	}

	void ParticleManager::Update(float DeltaTime)
	{
		GX_PROFILE_FUNCTION()
		
		GX_ENGINE_ASSERT(s_Data != nullptr, "Particle Manager is not Initialised");
		
		const GM::Matrix4& ViewMatrix = s_Data->Camera->GetViewMatrix();
		for (const auto& System : s_Data->ParticleSystems)
		{
			System.second->Update(DeltaTime, ViewMatrix);
		}
	}

	void ParticleManager::SpawnParticles(float DeltaTime)
	{
		GX_PROFILE_FUNCTION()

		if (!GX_ENABLE_PARTICLE_EFFECTS)
		{
			return;
		}

		for (const auto& System : s_Data->ParticleSystems)
		{
			if(System.second->IsActive())
				System.second->SpawnParticles(DeltaTime);
		}
	}

	void ParticleManager::RenderParticles()
	{
		GX_PROFILE_FUNCTION()

		GX_ENGINE_ASSERT(s_Data != nullptr, "Particle Manager is not Initialised");

		Renderer2D::RenderParticles(s_Data->ParticleSystems);
	}

	bool ParticleManager::Exists(const std::string& name)
	{
		return s_Data->ParticleSystems.find(name) != s_Data->ParticleSystems.end();
	}
}