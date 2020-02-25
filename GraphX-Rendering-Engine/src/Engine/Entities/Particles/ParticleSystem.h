#pragma once

#include "Particle.h"

namespace GraphX
{
	class ParticleManager;
	class Texture2D;

	struct ParticleSystemConfig
	{
		ParticleProps ParticleProperties;
		GM::Vector3 VelocityVariation;
		unsigned int ParticlesPerSec;
		float SizeVariation, LifeSpanVariation, GravityVariation;
	};

	/* Generates and renders the particles in the scene */
	class ParticleSystem
	{
	public:
		ParticleSystem(const Ref<ParticleManager>& Manager, const ParticleSystemConfig& props);

		/* Spawn Particles at the specified location */
		void SpawnParticles(const GM::Vector3& SpawnLocation, float DeltaTime);

		void SetParticleProperties(const ParticleProps& props) { m_Config.ParticleProperties = props; }

		void SetParticlePerSec(const unsigned int ParticlesPerSec) { m_Config.ParticlesPerSec = ParticlesPerSec; }

		/* Returns the current configuration for the particle system */
		inline const ParticleSystemConfig& GetConfig() const { return m_Config; }

	private:
		/* Emits a particle */
		void EmitParticle(ParticleProps& props);

		/* Generates a random value using the average value and the offset deviation factor */
		float GenerateRandomValue(float AverageValue, float Variation);

	private:
		/* To handle all the generated particles */
		Ref<ParticleManager> m_Manager;

		/* Current Configuration of the system */
		ParticleSystemConfig m_Config;
	};
}