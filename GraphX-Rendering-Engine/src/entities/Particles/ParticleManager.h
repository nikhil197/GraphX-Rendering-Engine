#pragma once

#include "Particle.h"

namespace engine
{
	class ParticleManager
	{
	private:
		/* Shader used to renderer the particles in this particle system*/
		class Shader* m_ParticleShader;

		/* All the Particles to be rendered */
		std::vector<Particle> m_Particles;

		/* Main Camera of the engine */
		const class Camera& m_Camera;

	public:
		ParticleManager(const class Camera& camera);

		/* Renders the particles on the screen */
		void RenderParticles();

		/* Updates all the particles */
		void Update(float DeltaTime);

		/* Adds a new particle to render */
		void AddParticle(const Particle& particle);

	private:
		/* Do Stuff before rendering the particles */
		void PreRender();

		/* Do Stuff after the particles are rendered*/
		void PostRender();
	};
}