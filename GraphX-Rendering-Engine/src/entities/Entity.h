#pragma once

namespace engine
{
	class Entity
	{
	protected:
		Entity()
			: m_Destroy(false)
		{};

		/* Whether the entity is marked to be destroyed */
		bool m_Destroy;

	public:
		/* Update the Entity */
		virtual void Update(float DeltaTime) = 0;

		/* Enable the Entity for rendering */
		virtual void Enable(class Shader& shader, const std::string& EntityNameInShader) const = 0;

		/* Disable the Entity after rendering */
		virtual void Disable() const = 0;

		/* Returns whether the entity is marked to be destroyed */
		inline bool IsToBeDestroyed() const { return m_Destroy; }
	};
}