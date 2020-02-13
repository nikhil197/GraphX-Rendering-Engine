#pragma once

#include "Particle.h"

namespace GraphX
{
	class Camera;
	class Shader;

	class ParticleManager
	{
	public:
		ParticleManager();

		~ParticleManager();

		/* Initialize the manager */
		void Initialize(const Ref<const Camera>& camera, const int Pool);

		/* Renders the particles on the screen */
		void RenderParticles();

		/* Updates all the particles */
		void Update(float DeltaTime);

		/* Adds a new particle to render */
		void AddParticle(const GM::Vector3& Position, const GM::Vector3& Velocity, float LifeSpan, float Rotation, const Ref<Texture>& texture, float Scale = 1.0f, float GravityEffect = 1.0f);

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

	private:
		/* Shader used to renderer the particles in this particle system*/
		Ref<Shader> m_ParticleShader;

		/* Particles pool */
		Scope<std::vector<Particle>> m_Particles;

		/* Main Camera of the engine */
		Ref<const Camera> m_Camera;

		/* Current Index in the pool which is supposed to be un-used */
		unsigned int m_Index;

		/* Capacity of the pool */
		unsigned int m_PoolCap;
	};
}