#pragma once

namespace GraphX
{
	/* Generates and renders the particles in the scene */
	class ParticleSystem
	{
	public:
		ParticleSystem(class ParticleManager& Manager, const class Texture& ParticleTexture, float ParticlesPerSec, float Speed, float GravityEffect, float LifeSpan, float Scale,
			float SpeedDeviation = 0.0f, float LifeSpanDeviation = 0.0f, float ScaleDeviation = 0.0f, float GravityEffectDeviation = 0.0f);

		/* Spawn Particles at the specified location */
		void SpawnParticles(const GM::Vector3& SpawnLocation, float DeltaTime);

	private:
		/* Emits a particle */
		void EmitParticle(const GM::Vector3& SpawnLocation);

		/* Generates a random value using the average value and the offset deviation factor */
		float GenerateRandomValue(float AverageValue, float Deviation);

	private:
		/* To handle all the generated particles */
		class ParticleManager& m_Manager;

		/* Texture used for the particles of this particle system */
		const class Texture& m_Texture;

		/* Particles emitted per second */
		float m_ParticlesPerSec;

		/* Average Speed of the particles */
		float m_Speed;

		/* Average Gravity effect on the particles */
		float m_GravityEffect;

		/* Average life span of the particles */
		float m_LifeSpan;

		/* Average Scale for the particles */
		float m_Scale;

		/* Deviation from the average speed */
		float m_SpeedDeviation;

		/* Deviation from the average scale */
		float m_ScaleDeviation;

		/* Deviation from the average particle life span */
		float m_LifeSpanDeviation;

		/* Deviation from the average gravity effect */
		float m_GravityEffectDeviation;
	};
}