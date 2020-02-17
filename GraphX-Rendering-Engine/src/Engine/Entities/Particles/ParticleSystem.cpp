#include "pch.h"
#include "ParticleSystem.h"

#include "ParticleManager.h"
#include "Utilities/EngineUtil.h"
#include "Textures/Texture2D.h"

namespace GraphX
{
	ParticleSystem::ParticleSystem(const Ref<ParticleManager>& Manager, const Ref<Texture2D>& ParticleTexture, float ParticlesPerSec, float Speed, float GravityEffect, float LifeSpan, float Scale, float SpeedDeviation, float LifeSpanDeviation, float ScaleDeviation, float GravityEffectDeviation)
		: m_Manager(Manager), m_Texture(ParticleTexture), m_ParticlesPerSec(ParticlesPerSec), m_Speed(Speed), m_GravityEffect(GravityEffect), m_LifeSpan(LifeSpan), m_Scale(Scale), m_SpeedDeviation(SpeedDeviation), m_ScaleDeviation(ScaleDeviation), m_LifeSpanDeviation(LifeSpanDeviation), m_GravityEffectDeviation(GravityEffectDeviation)
	{
	}

	void ParticleSystem::SpawnParticles(const GM::Vector3& SpawnLocation, float DeltaTime)
	{
		GX_PROFILE_FUNCTION()

		static int MaxParticlesPerFrame = (int) m_ParticlesPerSec;
		static int MinParticlesPerFrame = (int) m_ParticlesPerSec / 2;
		int ParticlesCount = (int)(m_ParticlesPerSec * DeltaTime);
		GM::Utility::Clamp<int>(ParticlesCount, MinParticlesPerFrame, MaxParticlesPerFrame);
		for (int i = 0; i < ParticlesCount && m_Manager->IsPoolEmpty(); i++)
		{
			EmitParticle(SpawnLocation);
		}
	}

	void ParticleSystem::EmitParticle(const GM::Vector3& SpawnLocation)
	{
		GX_PROFILE_FUNCTION()

		float LifeSpan = GenerateRandomValue(m_LifeSpan, m_LifeSpanDeviation);
		float Scale = GenerateRandomValue(m_Scale, m_ScaleDeviation);
		float GravityEffect = GenerateRandomValue(m_GravityEffect, m_GravityEffectDeviation);
		GM::Vector3 Velocity = GM::Vector3((float)EngineUtil::GetRandomValue() * 2.0f - 1.0f, 1.0f, (float)EngineUtil::GetRandomValue() * 2.0f - 1.0f);
		m_Manager->AddParticle(SpawnLocation, Velocity, LifeSpan, 0.0f, m_Texture, Scale, GravityEffect);
	}

	float ParticleSystem::GenerateRandomValue(float Average, float Deviation)
	{
		float Offset = Deviation * Average * ((float) EngineUtil::GetRandomValue() * 2 - 1.0f);
		return Average + Offset;
	}
}