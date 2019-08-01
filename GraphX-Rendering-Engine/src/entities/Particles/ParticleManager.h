#pragma once

#include "Particle.h"
#include "Utilities/HashUtil.h"

namespace GraphX
{
	class ParticleManager
	{
	private:
		/* Shader used to renderer the particles in this particle system*/
		class Shader* m_ParticleShader;

		/* Particles pool */
		std::vector<Particle>* m_Particles;

		/* Main Camera of the engine */
		const class Camera* m_Camera;

		/* Current Index in the pool which is supposed to be un-used */
		unsigned int m_Index;

		/* Capacity of the pool */
		unsigned int m_PoolCap;

	public:
		ParticleManager();

		~ParticleManager();

		/* Initialize the manager */
		void Initialize(const class Camera* camera, const int Pool);

		/* Renders the particles on the screen */
		void RenderParticles();

		/* Updates all the particles */
		void Update(float DeltaTime);

		/* Adds a new particle to render */
		void AddParticle(const GraphXMaths::Vector3& Position, const GraphXMaths::Vector3& Velocity, float LifeSpan, float Rotation, const class Texture* texture, float Scale = 1.0f, float GravityEffect = 1.0f);

		/* Whether more particles can be added or not */
		bool IsPoolEmpty() const { return !m_Particles->at(m_Index).IsUsed(); }

		void ResizeParticlesPool(const int NewPoolSize)
		{
			if (NewPoolSize != m_PoolCap)
			{
				m_PoolCap = NewPoolSize;
				m_Particles->resize(NewPoolSize);
			}
		}

	private:
		/* Do Stuff before rendering the particles */
		void PreRender();

		/* Do Stuff after the particles are rendered*/
		void PostRender();

		bool IsInitialized();
	};
}