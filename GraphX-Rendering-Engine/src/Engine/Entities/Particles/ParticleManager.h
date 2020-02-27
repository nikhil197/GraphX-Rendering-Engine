#pragma once

namespace GraphX
{
	class Camera;
	class Shader;
	class ParticleSystem;

	class ParticleManager
	{
	public:
		ParticleManager();

		~ParticleManager();

		/* Initialize the manager */
		void Initialize(const Ref<const Camera>& camera);

		/* Renders the particles on the screen */
		void RenderParticles();

		/* Updates all the particles */
		void Update(float DeltaTime);

		/* Spawn more particles for the current frame */
		void SpawnParticles(float DeltaTime);

		// Add a new particle system to the manager
		void AddParticleSystem(const Ref<ParticleSystem>& System);

		// Returns a particle system with given name
		Ref<ParticleSystem> GetParticleSystem(const std::string& name);

		/* Returns true if a particle system exists */
		bool Exists(const std::string& name) const;

	private:
		bool IsInitialized();

	private:
		/* Shader used to renderer the particles in this particle system*/
		Ref<Shader> m_ParticleShader;

		/* All particle systems in the world */
		std::unordered_map<std::string, Ref<ParticleSystem>> m_ParticleSystems;

		/* Main Camera of the engine */
		Ref<const Camera> m_Camera;

		/* Current Index in the pool which is supposed to be un-used */
		unsigned int m_Index;

		/* Capacity of the pool */
		unsigned int m_PoolCap;
	};
}