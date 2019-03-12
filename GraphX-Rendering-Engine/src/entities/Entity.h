#pragma once

namespace engine
{
	class Entity
	{
	public:
		/* Update the Entity */
		virtual void Update(float DeltaTime) = 0;

		/* Enable the Entity for rendering */
		virtual void Enable(class Shader& shader, const std::string& EntityNameInShader) const = 0;

		/* Disable the Entity after rendering */
		virtual void Disable() const = 0;
	};
}