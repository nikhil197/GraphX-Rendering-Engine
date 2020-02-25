#include "pch.h"
#include "Particle.h"

#include "Utilities/EngineConstants.h"
#include "Shaders/Shader.h"
#include "Textures/Texture2D.h"

namespace GraphX
{
	Particle::Particle()
		: Entity(), m_CurrentTexOffset(), m_NextTexOffset()
	{
	}

	Particle::Particle(const ParticleProps& props)
		: Entity()
	{
		Init(props);
	}

	void Particle::Init(const ParticleProps& props)
	{
		m_ElapsedTime = 0.0f;
		m_Props.ColorBegin		= props.ColorBegin;
		m_Props.ColorEnd		= props.ColorEnd;
		m_Props.Position		= props.Position;
		m_Props.Velocity		= props.Velocity;
		m_Props.LifeSpan		= props.LifeSpan;
		m_Props.Rotation		= props.Rotation;
		m_Props.Texture			= props.Texture;
		m_Props.SizeBegin		= props.SizeBegin;
		m_Props.SizeEnd			= props.SizeEnd;
		m_Props.GravityEffect = props.GravityEffect;
		m_Active = true;
	}

	void Particle::Update(float DeltaTime)
	{
	}

	void Particle::Update(float DeltaTime, const GM::Vector3& CameraViewSpacePos, bool UpdateMatrix)
	{
		m_ElapsedTime += DeltaTime;
		if (m_ElapsedTime >= m_Props.LifeSpan)
		{
			m_Active = false;
		}
		else if(m_Active)
		{
			float scale = GM::Utility::Lerp(m_Props.SizeBegin, m_Props.SizeEnd, m_ElapsedTime / m_Props.LifeSpan);
			m_Props.Velocity.y += GX_ENGINE_GRAVITY * m_Props.GravityEffect * DeltaTime;

			m_Props.Position += m_Props.Velocity;
			
			if (m_Props.Texture && m_Props.Texture->GetRowsInTexAtlas() > 1)
				UpdateTexOffset();

			// Change the model matrix based on ViewMatrix only if the view matrix is changed or if this is the first frame for the particle
			if (UpdateMatrix || m_ElapsedTime == DeltaTime)
			{
				m_Model = GM::Scaling(scale) * GM::Rotation(m_Props.Rotation, GM::Vector3::ZAxis) 
							* GM::Translation(m_Props.Position + CameraViewSpacePos);
			}
			else
			{
				m_Model *= GM::Scaling(scale) * GM::Translation(m_Props.Velocity * scale);
			}
		}
	}

	void Particle::Enable(Shader& shader, const std::string& EntityNameInShader) const
	{
		shader.SetUniformMat4f("u_Model", m_Model);
		
		if (m_Props.Texture)
		{
			m_Props.Texture->Bind();
			shader.SetUniform1i("u_ParticleTexture", 0);
			shader.SetUniform1i("u_TexAtlasRows", (int)m_Props.Texture->GetRowsInTexAtlas());
			
			if (m_Props.Texture->GetRowsInTexAtlas() > 1)
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

		float LifeSpanFactor = m_ElapsedTime / m_Props.LifeSpan;
		int TotalStages = GM::Utility::Square(m_Props.Texture->GetRowsInTexAtlas());
		float LifeProgress = LifeSpanFactor * TotalStages;
		int index1 = (int)LifeProgress;
		int index2 = (index1 + 1 < TotalStages) ? index1 + 1 : index1;
		m_BlendFactor = LifeProgress - index1;

		CalculateOffset(index1, m_CurrentTexOffset);
		CalculateOffset(index2, m_NextTexOffset);
	}

	void Particle::CalculateOffset(int index, GM::Vector2& TexOffset)
	{
		int column = index % m_Props.Texture->GetRowsInTexAtlas();
		int row = index / m_Props.Texture->GetRowsInTexAtlas();
		TexOffset.x = (float) column / (float) m_Props.Texture->GetRowsInTexAtlas();
		TexOffset.y = (float) row / (float) m_Props.Texture->GetRowsInTexAtlas();
	}
}