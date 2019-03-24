#include "pch.h"
#include "Particle.h"

#include "Utilities/EngineConstants.h"
#include "Shaders/Shader.h"
#include "Textures/Texture.h"

namespace engine
{
	const Quad& Particle::GetQuad()
	{
		static Quad s_Quad;
		return s_Quad;
	}

	Particle::Particle(const gm::Vector3& Position, const gm::Vector3& Velocity, float LifeSpan, float Rotation, const class Texture& Tex, float Scale, float GravityEffect)
		: Entity(), m_Position(Position), m_Velocity(Velocity), m_GravityEffect(GravityEffect), m_LifeSpan(LifeSpan), m_Rotation(Rotation), m_Scale(Scale), m_ElapsedTime(0.0f), m_Texture(&Tex), m_CurrentTexOffset(gm::Vector2()), m_NextTexOffset(gm::Vector2()), m_BlendFactor(0.0f)
	{
	}

	void Particle::Update(float DeltaTime)
	{
		m_Velocity.y += GX_ENGINE_GRAVITY * m_GravityEffect * DeltaTime;
		m_Position = m_Position + (m_Velocity * m_Scale);
		m_ElapsedTime += DeltaTime;
		UpdateTexOffset();
		if (m_ElapsedTime >= m_LifeSpan)
			m_Destroy = true;
	}

	void Particle::Enable(Shader& shader, const gm::Matrix4& ViewMatrix)
	{
		m_Texture->Bind();
		shader.SetUniform1i("u_ParticleTexture", 0);
		m_Model = gm::Translation(m_Position);
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				m_Model[i][j] = ViewMatrix[j][i];
		m_Model = gm::Scaling(m_Scale) * gm::Rotation(m_Rotation, gm::Vector3::ZAxis) * m_Model;
		shader.SetUniformMat4f("u_Model", m_Model);
		shader.SetUniform1f("u_BlendFactor", m_BlendFactor);
		shader.SetUniform1i("u_TexAtlasRows", (int)m_Texture->GetRowsInTexAtlas());
		shader.SetUniform2f("u_TexCoordOffset1", m_CurrentTexOffset);
		shader.SetUniform2f("u_TexCoordOffset2", m_NextTexOffset);
	}

	void Particle::Disable() const
	{
	}

	void Particle::Enable(class Shader& shader, const std::string& EntityNameInShader) const
	{
	}

	void Particle::UpdateTexOffset()
	{
		float LifeSpanFactor = m_ElapsedTime / m_LifeSpan;
		int TotalStages = gm::MathUtil::Square(m_Texture->GetRowsInTexAtlas());
		float LifeProgress = LifeSpanFactor * TotalStages;
		int index1 = (int)LifeProgress;
		int index2 = (index1 + 1 < TotalStages) ? index1 + 1 : index1;
		m_BlendFactor = LifeProgress - index1;

		CalculateOffset(index1, m_CurrentTexOffset);
		CalculateOffset(index2, m_NextTexOffset);
	}

	void Particle::CalculateOffset(int index, gm::Vector2& TexOffset)
	{
		int column = index % m_Texture->GetRowsInTexAtlas();
		int row = index / m_Texture->GetRowsInTexAtlas();
		TexOffset.x = (float) column / (float) m_Texture->GetRowsInTexAtlas();
		TexOffset.y = (float) row / (float) m_Texture->GetRowsInTexAtlas();
	}
}