#pragma once

#include "Entities/Entity.h"
#include "Model/Quad.h"

namespace engine
{
	class Particle
		: public Entity
	{
	public:
		/* Single quad to render all the particles */
		static const Quad& GetQuad();

	private:
		/* Position of the particle in the world */
		gm::Vector3 m_Position;

		/* Speed and the direction in which the particle is travelling */
		gm::Vector3 m_Velocity;

		/* Effect of the gravity on the particle (1 means full effect of the gravity) */
		float m_GravityEffect;

		/* Life span of the particle */
		float m_LifeSpan;

		/* Rotation applied to the particle */
		float m_Rotation;

		/* Scale of the particle */
		float m_Scale;

		/* Time elapsed since the creation of the particle */
		float m_ElapsedTime;

		/* Model matrix for the particle */
		gm::Matrix4 m_Model;

		/* Texture (Atlas) used for the particle */
		const class Texture* m_Texture;

		/* Offset of the current texture to be used in the atlas */
		gm::Vector2 m_CurrentTexOffset;

		/* Offset of the next texture to be used in the atlas */
		gm::Vector2 m_NextTexOffset;

		/* Factor used to blend between the two textures */
		float m_BlendFactor;

	public:
		Particle(const gm::Vector3& Position, const gm::Vector3& Velocity, float LifeSpan, float Rotation, const class Texture& Texture, float Scale = 1.0f, float GravityEffect = 1.0f);

		/* Entity Interface */
		void Update(float DeltaTime, const gm::Matrix4& ViewMatrix, bool UpdateMatrix);

		virtual void Enable(class Shader& shader, const std::string& EntityNameInShader = "") const override;
		
		virtual void Disable() const override;

		/* Returns the Position of the particle */
		inline const gm::Vector3& GetPosition() const { return m_Position; }

		/* Returns rotation of the particle */
		inline float GetRotation() const { return m_Rotation; }

		/* Returns the scale of the particle */
		inline float GetScale() const { return m_Scale; }

		/* Returns the texture(atlas) of the particle */
		inline const class Texture& GetTexture() const { return *m_Texture; }

	private:
		void Update(float DeltaTime) override;
		
		/* Updated the texture atlas offset for the particle based on the particle lifespan */
		void UpdateTexOffset();

		/* Calculates the offset for the given index */
		void CalculateOffset(int index, gm::Vector2& TexOffset);
	};
}