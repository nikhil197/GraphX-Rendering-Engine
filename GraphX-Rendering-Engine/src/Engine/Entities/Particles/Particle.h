#pragma once

#include "Entities/Entity.h"
#include "Model/Quad.h"

namespace GraphX
{
	class Texture;
	class Shader;

	class Particle
		: public Entity
	{
	public:
		/* Single quad to render all the particles */
		static const Quad& GetQuad();

	public:
		/* Default Consturctor for the particle pool */
		Particle();

		Particle(const GM::Vector3& Position, const GM::Vector3& Velocity, float LifeSpan, float Rotation, const Ref<Texture>& Texture, float Scale = 1.0f, float GravityEffect = 1.0f);

		/* To Initialise the particles that are defualt constructed */
		void Init(const GM::Vector3& Position, const GM::Vector3& Velocity, float LifeSpan, float Rotation, const Ref<Texture>& texture, float Scale = 1.0f, float GravityEffect = 1.0f);

		/* Entity Interface */
		void Update(float DeltaTime, const GM::Matrix4& ViewMatrix, bool UpdateMatrix);

		virtual void Enable(Shader& shader, const std::string& EntityNameInShader = "") const override;
		
		virtual void Disable() const override;

		/* Returns the Position of the particle */
		inline const GM::Vector3& GetPosition() const { return m_Position; }

		/* Returns rotation of the particle */
		inline float GetRotation() const { return m_Rotation; }

		/* Returns the scale of the particle */
		inline float GetScale() const { return m_Scale; }

		/* Returns the texture(atlas) of the particle */
		inline const Ref<Texture>& GetTexture() const { return m_Texture; }

		/* Whether the particles is being used or not */
		inline bool IsUsed() const { return m_Used; }

	private:
		void Update(float DeltaTime) override;
		
		/* Updated the texture atlas offset for the particle based on the particle lifespan */
		void UpdateTexOffset();

		/* Calculates the offset for the given index */
		void CalculateOffset(int index, GM::Vector2& TexOffset);

	private:
		/* Position of the particle in the world */
		GM::Vector3 m_Position;

		/* Speed and the direction in which the particle is travelling */
		GM::Vector3 m_Velocity;

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
		GM::Matrix4 m_Model;

		/* Texture (Atlas) used for the particle */
		Ref<Texture> m_Texture;

		/* Offset of the current texture to be used in the atlas */
		GM::Vector2 m_CurrentTexOffset;

		/* Offset of the next texture to be used in the atlas */
		GM::Vector2 m_NextTexOffset;

		/* Factor used to blend between the two textures */
		float m_BlendFactor;

		/* Whether the current particle is being used or not (Used specially for particles pool) */
		bool m_Used;
	};
}