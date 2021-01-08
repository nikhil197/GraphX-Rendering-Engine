#include "pch.h"
#include "Particle.h"

#include "Utilities/EngineConstants.h"
#include "Shaders/Shader.h"
#include "Textures/Texture2D.h"

#include "Core/Textures/SpriteSheet.h"
#include "Core/Textures/SubTexture2D.h"

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
		m_Props.ColorBegin = props.ColorBegin;
		m_Props.ColorEnd = props.ColorEnd;
		m_Props.Position = props.Position;
		m_Props.Velocity = props.Velocity;
		m_Props.LifeSpan = props.LifeSpan;
		m_Props.Rotation = props.Rotation;
		m_Props.Texture = props.Texture;
		m_Props.SizeBegin = props.SizeBegin;
		m_Props.SizeEnd = props.SizeEnd;
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
		else if (m_Active)
		{
			float scale = GM::Utility::Lerp(m_Props.SizeBegin, m_Props.SizeEnd, m_ElapsedTime / m_Props.LifeSpan);
			m_Props.Velocity.z += EngineConstants::GravityValue * m_Props.GravityEffect * DeltaTime;

			m_Props.Position += m_Props.Velocity * 0.1f;

			if (m_Props.Texture && m_Props.Texture->IsSpriteSheet())
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
			if (m_Props.Texture->GetRowsInAtlas() > 1)
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
		// No Need to check active status and texture
		// This method will be called only if the texture is a sprite sheet
		SpriteSheet* spriteSheet = static_cast<SpriteSheet*>(m_Props.Texture.get());

		float LifeSpanFactor = m_ElapsedTime / m_Props.LifeSpan;
		uint32_t TotalStages = spriteSheet->GetNumSprites();
		float LifeProgress = LifeSpanFactor * TotalStages;
		uint32_t index1 = (uint32_t)LifeProgress;
		uint32_t index2;

		if (index1 < TotalStages - 1)
			index2 = index1 + 1;
		else
			index2 = index1;

		// Set the texture coordinate offsets
		Ref<SubTexture2D> subtexture1 = spriteSheet->GetSprite(index1);
		const GM::Vector2* TexCoords = subtexture1->GetTexCoords();

		// Offset for 1st sprite
		m_TexOffset.x = TexCoords[0].x;
		m_TexOffset.y = TexCoords[0].y;

		// offset for 2nd sprite
		if (index1 == index2)
		{
			m_TexOffset.z = m_TexOffset.x;
			m_TexOffset.w = m_TexOffset.y;
		}
		else
		{
			Ref<SubTexture2D> subtexture2 = spriteSheet->GetSprite(index2);
			TexCoords = subtexture2->GetTexCoords();
			m_TexOffset.z = TexCoords[0].x;
			m_TexOffset.w = TexCoords[0].y;
		}

		// Factor by which to blend between the two sprites
		m_BlendFactor = LifeProgress - index1;
	}
}