#pragma once

#include "Particle.h"
#include "Utilities/HashUtil.h"

namespace engine
{
	typedef std::unordered_map < std::reference_wrapper<Texture>, std::vector<Particle>> ParticlesMap;

	class ParticleManager
	{
	private:
		/* Shader used to renderer the particles in this particle system*/
		class Shader* m_ParticleShader;

		/* All the Particles to be rendered */
		//ParticlesMap m_Particle;

		/* Particles pool */
		std::vector<Particle> m_Particles;

		/* Main Camera of the engine */
		const class Camera& m_Camera;

		/* Current Index in the pool which is supposed to be un-used */
		unsigned int m_Index;

		/* Capacity of the pool */
		unsigned int m_PoolCap;

	public:
		ParticleManager(const class Camera& camera);

		/* Renders the particles on the screen */
		void RenderParticles();

		/* Updates all the particles */
		void Update(float DeltaTime);

		/* Adds a new particle to render */
		void AddParticle(const Particle& particle);

		void AddParticle(const gm::Vector3& Position, const gm::Vector3& Velocity, float LifeSpan, float Rotation, const class Texture* texture, float Scale = 1.0f, float GravityEffect = 1.0f);

		/* Whether more particles can be added or not */
		bool IsPoolEmpty() const { return !m_Particles.at(m_Index).IsUsed(); }

	private:
		/* Do Stuff before rendering the particles */
		void PreRender();

		/* Do Stuff after the particles are rendered*/
		void PostRender();
	};
}