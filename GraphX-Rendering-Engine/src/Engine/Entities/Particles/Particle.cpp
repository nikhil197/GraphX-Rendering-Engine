#include "pch.h"
#include "Particle.h"

#include "Utilities/EngineConstants.h"
#include "Shaders/Shader.h"
#include "Textures/Texture2D.h"

namespace GraphX
{
	Particle::Particle()
		: Entity(), m_Position(), m_Velocity(), m_GravityEffect(1.0f), m_LifeSpan(0.0f), m_Rotation(0.0f), m_Scale(1.0f), m_ElapsedTime(0.0f), m_Texture(nullptr), m_CurrentTexOffset(), m_NextTexOffset(), m_BlendFactor(0.0f), m_Active(false)
	{
	}

	Particle::Particle(const GM::Vector3& Position, const GM::Vector3& Velocity, float LifeSpan, float Rotation, const Ref<Texture2D>& Tex, float Scale, float GravityEffect)
		: Entity(), m_Position(Position), m_Velocity(Velocity), m_GravityEffect(GravityEffect), m_LifeSpan(LifeSpan), m_Rotation(Rotation), m_Scale(Scale), m_ElapsedTime(0.0f), m_Texture(Tex), m_CurrentTexOffset(GM::Vector2()), m_NextTexOffset(GM::Vector2()), m_BlendFactor(0.0f), m_Active(false)
	{
	}

	void Particle::Init(const GM::Vector3& Position, const GM::Vector3& Velocity, float LifeSpan, float Rotation, const Ref<Texture2D>& texture, float Scale, float GravityEffect)
	{
		m_ElapsedTime = 0.0f;
		m_Position = Position;
		m_Velocity = Velocity;
		m_LifeSpan = LifeSpan;
		m_Rotation = Rotation;
		m_Texture = texture;
		m_Scale = Scale;
		m_GravityEffect = GravityEffect;
		m_Active = true;
	}

	void Particle::Update(float DeltaTime)
	{
	}

	void Particle::Update(float DeltaTime, const GM::Vector3& CameraViewSpacePos, bool UpdateMatrix)
	{
		m_ElapsedTime += DeltaTime;
		if (m_ElapsedTime >= m_LifeSpan)
		{
			//m_Destroy = true;
			m_Active = false;
			m_Texture = nullptr;
			m_ElapsedTime = 0.0f;
		}
		else if(m_Active)
		{
			m_Velocity.y += GX_ENGINE_GRAVITY * m_GravityEffect * DeltaTime;
			m_Position = m_Position + (m_Velocity * m_Scale);
			if (m_Texture && m_Texture->GetRowsInTexAtlas() > 1)
				UpdateTexOffset();

			// Change the model matrix based on ViewMatrix only if the view matrix is changed or if this is the first frame for the particle
			if (UpdateMatrix || m_ElapsedTime == DeltaTime)
			{
				m_Model = GM::Scaling(m_Scale) * GM::Rotation(m_Rotation, GM::Vector3::ZAxis) * GM::Translation(m_Position + CameraViewSpacePos);
			}
			else
			{
				m_Model *= GM::Translation(m_Velocity * m_Scale);
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
			
			if (m_Texture->GetRowsInTexAtlas() > 1)
			{
				shader.SetUniform1f("u_BlendFactor", m_BlendFactor);
				shader.SetUniform4f("u_TexCoordOffsets", m_CurrentTexOffset, m_NextTexOffset);
			}
		}
	}

	void Particle::Disable() const
	{
	}

	void Particle::UpdateTexOffset()
	{
		// No Need to check active status and texture. Will only be called if both of those are true

		float LifeSpanFactor = m_ElapsedTime / m_LifeSpan;
		int TotalStages = GM::Utility::Square(m_Texture->GetRowsInTexAtlas());
		float LifeProgress = LifeSpanFactor * TotalStages;
		int index1 = (int)LifeProgress;
		int index2 = (index1 + 1 < TotalStages) ? index1 + 1 : index1;
		m_BlendFactor = LifeProgress - index1;

		CalculateOffset(index1, m_CurrentTexOffset);
		CalculateOffset(index2, m_NextTexOffset);
	}

	void Particle::CalculateOffset(int index, GM::Vector2& TexOffset)
	{
		int column = index % m_Texture->GetRowsInTexAtlas();
		int row = index / m_Texture->GetRowsInTexAtlas();
		TexOffset.x = (float) column / (float) m_Texture->GetRowsInTexAtlas();
		TexOffset.y = (float) row / (float) m_Texture->GetRowsInTexAtlas();
	}
}