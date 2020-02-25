#include "pch.h"
#include "ParticleSystem.h"

#include "ParticleManager.h"
#include "Utilities/EngineUtil.h"
#include "Textures/Texture2D.h"

namespace GraphX
{
	ParticleSystem::ParticleSystem(const Ref<ParticleManager>& Manager, const ParticleSystemConfig& Config)
		: m_Manager(Manager), m_Config(Config)
	{
	}

	void ParticleSystem::SpawnParticles(const GM::Vector3& SpawnLocation, float DeltaTime)
	{
		GX_PROFILE_FUNCTION()

		static int MaxParticlesPerFrame = m_Config.ParticlesPerSec;
		static int MinParticlesPerFrame = m_Config.ParticlesPerSec / 2;
		int ParticlesCount = (int)(m_Config.ParticlesPerSec * DeltaTime);
		GM::Utility::Clamp<int>(ParticlesCount, MinParticlesPerFrame, MaxParticlesPerFrame);

		ParticleProps props = m_Config.ParticleProperties;
		props.Position = SpawnLocation;
		for (int i = 0; i < ParticlesCount && m_Manager->IsPoolEmpty(); i++)
		{
			EmitParticle(props);
		}
	}

	void ParticleSystem::EmitParticle(ParticleProps& props)
	{
		GX_PROFILE_FUNCTION()

		props.LifeSpan = GenerateRandomValue(m_Config.ParticleProperties.LifeSpan, m_Config.LifeSpanVariation);
		props.SizeBegin = GenerateRandomValue(m_Config.ParticleProperties.SizeBegin, m_Config.SizeVariation);
		props.GravityEffect = GenerateRandomValue(m_Config.ParticleProperties.GravityEffect, m_Config.GravityVariation);
		props.Velocity.x = m_Config.ParticleProperties.Velocity.x * m_Config.VelocityVariation.x * ((float)EngineUtil::GetRandomValue() * 2.0f - 1.0f);
		props.Velocity.y = m_Config.ParticleProperties.Velocity.y * m_Config.VelocityVariation.y;
		props.Velocity.z = m_Config.ParticleProperties.Velocity.z * m_Config.VelocityVariation.z * ((float)EngineUtil::GetRandomValue() * 2.0f - 1.0f);
		m_Manager->EmitParticle(props);
	}

	float ParticleSystem::GenerateRandomValue(float Average, float Deviation)
	{
		float Offset = Deviation * Average * ((float) EngineUtil::GetRandomValue() * 2 - 1.0f);
		return Average + Offset;
	}
}