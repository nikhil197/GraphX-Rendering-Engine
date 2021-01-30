#include "pch.h"
#include "ParticleSystem.h"

#include "ParticleManager.h"
#include "Utilities/EngineUtil.h"
#include "Textures/Texture2D.h"

namespace GraphX
{
	ParticleSystem::ParticleSystem(const std::string& name, const ParticleSystemConfig& Config, const GM::Vector3& Pos)
		: Position(Pos), m_Name(name), m_Config(Config), m_Index(0)
	{
		m_Particles.resize(m_Config.PoolCap);
	}

	void ParticleSystem::Update(float DeltaTime, const GM::Matrix4& ViewMatrix)
	{
		GX_PROFILE_FUNCTION()

		for (auto& Particle : m_Particles)
		{
			Particle.Update(DeltaTime, ViewMatrix);
		}
	}

	void ParticleSystem::SpawnParticles(float DeltaTime)
	{
		GX_PROFILE_FUNCTION()

		DeltaTime *= 10;
		
		float MinParticles = 0.5f * m_Config.ParticlesPerSec * DeltaTime;
		float MaxParticles = m_Config.ParticlesPerSec * DeltaTime;

		int ParticlesCount = (int)EngineUtil::RandRange(MinParticles, MaxParticles);
		
		ParticleProps props = m_Config.ParticleProperties;
		props.Position = Position;
		for (int i = 0; i < ParticlesCount && !m_Particles[m_Index].IsActive(); i++)
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
		props.Velocity.x = m_Config.ParticleProperties.Velocity.x * m_Config.VelocityVariation.x * (EngineUtil::Rand<float>() * 2.0f - 1.0f);
		props.Velocity.y = m_Config.ParticleProperties.Velocity.y * m_Config.VelocityVariation.y * (EngineUtil::Rand<float>() * 2.0f - 1.0f);
		props.Velocity.z = m_Config.ParticleProperties.Velocity.z * m_Config.VelocityVariation.z * (EngineUtil::Rand<float>() * 2.0f - 1.0f);
		
		m_Particles[m_Index].Init(props);
		m_Index = (m_Index + 1) % m_Config.PoolCap;
	}

	float ParticleSystem::GenerateRandomValue(float Average, float Deviation)
	{
		float Offset = Deviation * Average * (EngineUtil::Rand<float>() * 2 - 1.0f);
		return Average + Offset;
	}
}