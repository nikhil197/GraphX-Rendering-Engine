#pragma once

namespace GraphX
{
	class Camera;
	class Shader;
	class ParticleSystem;

	class ParticleManager
	{
	public:
		/* Initialize the manager */
		static void Init(const Ref<const Camera>& camera);

		/* Shutdown the particle manager */
		static void Shutdown();

		/* Renders the particles on the screen */
		static void RenderParticles();

		/* Updates all the particles */
		static void Update(float DeltaTime);

		/* Spawn more particles for the current frame */
		static void SpawnParticles(float DeltaTime);

		// Add a new particle system to the manager
		static void AddParticleSystem(const Ref<ParticleSystem>& System);

		// Returns a particle system with given name
		static Ref<ParticleSystem> GetParticleSystem(const std::string& name);

		/* Returns true if a particle system exists */
		static bool Exists(const std::string& name);

	private:
		struct ParticleManagerData
		{
			/* All particle systems in the world */
			std::unordered_map<std::string, Ref<ParticleSystem>> ParticleSystems;

			/* Main Camera of the engine */
			Ref<const Camera> Camera;
		};

		/* Particle Manager Data */
		static ParticleManagerData* s_Data;
	};
}