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
		unsigned int PoolCap = 1000;
		float SizeVariation, LifeSpanVariation, GravityVariation;
	};

	/* Generates and renders the particles in the scene */
	class ParticleSystem
	{
	public:
		ParticleSystem(const std::string& name, const ParticleSystemConfig& props, const GM::Vector3& Pos);

		void Update(float DeltaTime, const GM::Vector3& CameraViewSpacePos, bool UpdateMatrix);

		/* Spawn Particles at the specified location */
		void SpawnParticles(float DeltaTime);

		void SetParticleProperties(const ParticleProps& props) { m_Config.ParticleProperties = props; }

		void SetParticlePerSec(const unsigned int ParticlesPerSec) { m_Config.ParticlesPerSec = ParticlesPerSec; }

		inline const std::string& GetName() const { return m_Name; }

		/* Returns the current configuration for the particle system */
		inline const ParticleSystemConfig& GetConfig() const { return m_Config; }

		inline bool IsActive() const { return m_Active; }

		inline void SetActive(bool active) { m_Active = active; }

		void ResizeParticlesPool(const int NewPoolSize)
		{
			if (NewPoolSize != m_Config.PoolCap)
			{
				m_Config.PoolCap = NewPoolSize;
				m_Particles.resize(NewPoolSize);
			}
		}

		// For iterating over the particles
		std::vector<Particle>::const_iterator begin() const { return m_Particles.begin(); }
		std::vector<Particle>::const_iterator end() const { return m_Particles.end(); }
		
		std::vector<Particle>::iterator begin() { return m_Particles.begin(); }
		std::vector<Particle>::iterator end() { return m_Particles.end(); }


	private:
		/* Emits a particle */
		void EmitParticle(ParticleProps& props);

		/* Generates a random value using the average value and the offset deviation factor */
		float GenerateRandomValue(float AverageValue, float Variation);

	public:
		GM::Vector3 Position;

	private:
		/* Used by particle manager */
		std::string m_Name;

		/* Current Configuration of the system */
		ParticleSystemConfig m_Config;

		/* Particles pool */
		std::vector<Particle> m_Particles;

		/* Current Index in the pool which is supposed to be un-used */
		unsigned int m_Index;

		/* If the particle system is active or not */
		bool m_Active = true;
	};
}