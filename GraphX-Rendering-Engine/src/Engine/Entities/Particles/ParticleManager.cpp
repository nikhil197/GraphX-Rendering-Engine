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
		s_Data->ParticleShader = Renderer::GetShaderLibrary().Load("res/Shaders/ParticleShader.glsl", "Particle");
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

		if (!GX_ENABLE_BATCH_RENDERING && s_Data->Camera->IsRenderStateDirty())
		{
			s_Data->ParticleShader->Bind();
			s_Data->ParticleShader->SetUniformMat4f("u_Projection", s_Data->Camera->GetProjectionMatrix());
		}
		
		const GM::Matrix4& ViewMatrix = s_Data->Camera->GetViewMatrix();
		const GM::Vector3 CameraViewSpacePos(ViewMatrix(0, 3), ViewMatrix(1, 3), ViewMatrix(2, 3));
		for (const auto& System : s_Data->ParticleSystems)
		{
			System.second->Update(DeltaTime, CameraViewSpacePos, s_Data->Camera->IsRenderStateDirty());
		}
	}

	void ParticleManager::SpawnParticles(float DeltaTime)
	{
		GX_PROFILE_FUNCTION()

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