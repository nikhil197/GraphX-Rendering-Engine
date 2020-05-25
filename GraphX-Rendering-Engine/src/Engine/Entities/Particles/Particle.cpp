#include "pch.h"
#include "Particle.h"

#include "Utilities/EngineConstants.h"
#include "Shaders/Shader.h"
#include "Textures/Texture2D.h"

namespace GraphX
{
	Particle::Particle()
		: Entity(), m_TexOffset()
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

			// Change the model matrix only if batch rendering is not enabled
			if (!GX_ENABLE_BATCH_RENDERING)
			{
				// Change the model matrix based on ViewMatrix only if the view matrix is changed or if this is the first frame for the particle
				if (UpdateMatrix || m_ElapsedTime == DeltaTime)
				{
					GM::ScaleRotationTranslationMatrix::Make(m_Model, GM::Vector3(scale), m_Props.Rotation, GM::Vector3::ZAxis, m_Props.Position + CameraViewSpacePos);
					/*m_Model = GM::ScaleMatrix(scale) * GM::RotationMatrix(m_Props.Rotation, GM::Vector3::ZAxis)
						* GM::TranslationMatrix(m_Props.Position + CameraViewSpacePos);*/
				}
				else
				{
					m_Model *= GM::ScaleRotationTranslationMatrix(GM::Vector3(scale), GM::Rotator::ZeroRotator, m_Props.Velocity * scale); 
					/*GM::ScaleMatrix(scale)* GM::TranslationMatrix(m_Props.Velocity * scale);*/
				}
			}
		}
	}

	void Particle::Enable(Shader& shader, const std::string& EntityNameInShader) const
	{
		shader.SetUniformMat4f("u_Model", m_Model);
		
		if (m_Props.Texture)
		{
			if (m_Props.Texture->GetRowsInTexAtlas() > 1)
			{
				shader.SetUniform1f("u_BlendFactor", m_BlendFactor);
				shader.SetUniform4f("u_TexCoordOffsets", m_TexOffset);
			}
		}
		else
		{
			GM::Vector4 color = GM::Utility::Lerp(m_Props.ColorBegin, m_Props.ColorEnd, m_ElapsedTime / m_Props.LifeSpan);
			shader.SetUniform4f("u_Color", color);
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

		CalculateOffset(index1, m_TexOffset.x, m_TexOffset.y);
		CalculateOffset(index2, m_TexOffset.z, m_TexOffset.w);
	}

	void Particle::CalculateOffset(int index, float& xTexOffset, float& yTexOffset)
	{
		int column = index % m_Props.Texture->GetRowsInTexAtlas();
		int row = index / m_Props.Texture->GetRowsInTexAtlas();
		xTexOffset = (float) column / (float) m_Props.Texture->GetRowsInTexAtlas();
		yTexOffset = (float) row / (float) m_Props.Texture->GetRowsInTexAtlas();
	}
}