#include "pch.h"
#include "Particle.h"

#include "Utilities/EngineConstants.h"
#include "Shaders/Shader.h"

namespace engine
{
	const Quad& Particle::GetQuad()
	{
		static Quad s_Quad;
		return s_Quad;
	}

	Particle::Particle(const gm::Vector3& Position, const gm::Vector3& Velocity, float LifeSpan, float Rotation, float Scale, float GravityEffect)
		: Entity(), m_Position(Position), m_Velocity(Velocity), m_GravityEffect(GravityEffect), m_LifeSpan(LifeSpan), m_Rotation(Rotation), m_Scale(Scale), m_ElapsedTime(0.0f)
	{
	}

	void Particle::Update(float DeltaTime)
	{
		m_Velocity.y += GX_ENGINE_GRAVITY * m_GravityEffect * DeltaTime;
		m_Position = m_Position + (m_Velocity * m_Scale);
		m_ElapsedTime += DeltaTime;
		if (m_ElapsedTime >= m_LifeSpan)
			m_Destroy = true;
	}

	void Particle::Enable(Shader& shader, const gm::Matrix4& ViewMatrix)
	{
		m_Model = gm::Translation(m_Position);
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				m_Model[i][j] = ViewMatrix[j][i];
		m_Model = gm::Scaling(m_Scale) * gm::Rotation(m_Rotation, gm::Vector3::ZAxis) * m_Model;
		shader.SetUniformMat4f("u_Model", m_Model);
	}

	void Particle::Disable() const
	{
	}

	void Particle::Enable(class Shader& shader, const std::string& EntityNameInShader) const
	{
	}
}