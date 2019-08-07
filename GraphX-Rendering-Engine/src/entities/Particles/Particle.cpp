#include "pch.h"
#include "Particle.h"

#include "Utilities/EngineConstants.h"
#include "Shaders/Shader.h"
#include "Textures/Texture.h"

namespace GraphX
{
	const Quad& Particle::GetQuad()
	{
		static Quad s_Quad;
		return s_Quad;
	}

	Particle::Particle()
		: Entity(), m_Position(), m_Velocity(), m_GravityEffect(1.0f), m_LifeSpan(0.0f), m_Rotation(0.0f), m_Scale(1.0f), m_ElapsedTime(0.0f), m_Texture(nullptr), m_CurrentTexOffset(), m_NextTexOffset(), m_BlendFactor(0.0f), m_Used(false)
	{
	}

	Particle::Particle(const GraphXMaths::Vector3& Position, const GraphXMaths::Vector3& Velocity, float LifeSpan, float Rotation, const class Texture& Tex, float Scale, float GravityEffect)
		: Entity(), m_Position(Position), m_Velocity(Velocity), m_GravityEffect(GravityEffect), m_LifeSpan(LifeSpan), m_Rotation(Rotation), m_Scale(Scale), m_ElapsedTime(0.0f), m_Texture(&Tex), m_CurrentTexOffset(GraphXMaths::Vector2()), m_NextTexOffset(GraphXMaths::Vector2()), m_BlendFactor(0.0f), m_Used(false)
	{
	}

	void Particle::Init(const GraphXMaths::Vector3& Position, const GraphXMaths::Vector3& Velocity, float LifeSpan, float Rotation, const class Texture* texture, float Scale, float GravityEffect)
	{
		m_ElapsedTime = 0.0f;
		m_Position = Position;
		m_Velocity = Velocity;
		m_LifeSpan = LifeSpan;
		m_Rotation = Rotation;
		m_Texture = texture;
		m_Scale = Scale;
		m_GravityEffect = GravityEffect;
		m_Used = true;
	}

	void Particle::Update(float DeltaTime)
	{
	}

	void Particle::Update(float DeltaTime, const GraphXMaths::Matrix4& ViewMatrix, bool UpdateMatrix)
	{
		m_ElapsedTime += DeltaTime;
		if (m_ElapsedTime >= m_LifeSpan)
		{
			//m_Destroy = true;
			m_Used = false;
			m_Texture = nullptr;
			m_ElapsedTime = 0.0f;
		}
		else if(m_Used)
		{
			m_Velocity.y += GX_ENGINE_GRAVITY * m_GravityEffect * DeltaTime;
			m_Position = m_Position + (m_Velocity * m_Scale);
			if (m_Texture && m_Texture->GetRowsInTexAtlas() > 1)
				UpdateTexOffset();

			// Change the model matrix based on ViewMatrix only if the view matrix is changed or if this is the first frame for the particle
			if (UpdateMatrix || m_ElapsedTime == DeltaTime)
			{
				m_Model = GraphXMaths::Translation(m_Position);
				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++)
						m_Model[i][j] = ViewMatrix[j][i];
				m_Model = GraphXMaths::Scaling(m_Scale) * GraphXMaths::Rotation(m_Rotation, GraphXMaths::Vector3::ZAxis) * m_Model;
			}
			else
			{
				m_Model *= GraphXMaths::Translation(m_Velocity * m_Scale);
			}
		}
	}

	void Particle::Enable(Shader& shader, const std::string& EntityNameInShader) const
	{
		shader.SetUniformMat4f("u_Model", m_Model);
		
		if (m_Texture)
		{
			m_Texture->Bind();
			shader.SetUniform1i("u_ParticleTexture", 0);
			shader.SetUniform1i("u_TexAtlasRows", (int)m_Texture->GetRowsInTexAtlas());
		}

		if (m_Texture && m_Texture->GetRowsInTexAtlas() > 1)
		{
			shader.SetUniform1f("u_BlendFactor", m_BlendFactor);
			shader.SetUniform4f("u_TexCoordOffsets", m_CurrentTexOffset, m_NextTexOffset);
		}
	}

	void Particle::Disable() const
	{
	}

	void Particle::UpdateTexOffset()
	{
		if (m_Used && m_Texture)
		{
			float LifeSpanFactor = m_ElapsedTime / m_LifeSpan;
			int TotalStages = GraphXMaths::Utility::Square(m_Texture->GetRowsInTexAtlas());
			float LifeProgress = LifeSpanFactor * TotalStages;
			int index1 = (int)LifeProgress;
			int index2 = (index1 + 1 < TotalStages) ? index1 + 1 : index1;
			m_BlendFactor = LifeProgress - index1;

			CalculateOffset(index1, m_CurrentTexOffset);
			CalculateOffset(index2, m_NextTexOffset);
		}
	}

	void Particle::CalculateOffset(int index, GraphXMaths::Vector2& TexOffset)
	{
		int column = index % m_Texture->GetRowsInTexAtlas();
		int row = index / m_Texture->GetRowsInTexAtlas();
		TexOffset.x = (float) column / (float) m_Texture->GetRowsInTexAtlas();
		TexOffset.y = (float) row / (float) m_Texture->GetRowsInTexAtlas();
	}
}