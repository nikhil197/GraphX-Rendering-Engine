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
	public:
		Particle(const gm::Vector3& Position, const gm::Vector3& Velocity, float LifeSpan, float Rotation, float Scale = 1.0f, float GravityEffect = 1.0f);

		/* Entity Interface */
		void Update(float DeltaTime) override;

		void Enable(Shader& shader, const gm::Matrix4& ViewMatrix);

		virtual void Disable() const override;

		/* Returns the Position of the particle */
		inline const gm::Vector3& GetPosition() const { return m_Position; }

		/* Returns rotation of the particle */
		inline float GetRotation() const { return m_Rotation; }

		/* Returns the scale of the particle */
		inline float GetScale() const { return m_Scale; }

	private:
		void Enable(class Shader& shader, const std::string& EntityNameInShader) const override;
	};
}