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
		: Entity(), m_Index1(0), m_Index2(0)
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
		m_Index1 = m_Index2 = 0;
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

	void Particle::Update(float DeltaTime, const GM::Matrix4& ViewMatrix)
	{
		m_ElapsedTime += DeltaTime;
		if (m_ElapsedTime >= m_Props.LifeSpan)
		{
			m_Active = false;
		}
		else if (m_Active)
		{
			float scale = GM::Utility::Lerp(m_Props.SizeBegin, m_Props.SizeEnd, m_ElapsedTime / m_Props.LifeSpan);
			m_Props.Velocity.y += EngineConstants::GravityValue * m_Props.GravityEffect * DeltaTime;

			m_Props.Position += m_Props.Velocity * 0.1f;

			if (m_Props.Texture && m_Props.Texture->IsSpriteSheet())
				UpdateTexOffset();

			// Change the model matrix only if batch rendering is not enabled
			if (!GX_ENABLE_BATCH_RENDERING)
			{
				GM::Rotator ParticleRotation(0.0f, 0.0f, m_Props.Rotation);
				GM::Vector3 Position = ViewMatrix * m_Props.Position;
				GM::ScaleRotationTranslationMatrix::Make(m_Model, GM::Vector3(scale, scale, 1.0f), ParticleRotation, Position);
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

				// Calculate the texture offsets
				SpriteSheet* spriteSheet = static_cast<SpriteSheet*>(m_Props.Texture.get());
				const GM::Vector2* TexCoords1 = spriteSheet->GetSprite(m_Index1)->GetTexCoords();
				const GM::Vector2* TexCoords2 = spriteSheet->GetSprite(m_Index2)->GetTexCoords();

				GM::Vector4 TexOffsets(TexCoords1[0].x, TexCoords1[0].y, TexCoords2[0].x, TexCoords2[0].y);
				shader.SetUniform4f("u_TexCoordOffsets", TexOffsets);
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
		m_Index1 = (uint32_t)LifeProgress;
		m_Index2;

		if (m_Index1 < TotalStages - 1)
			m_Index2 = m_Index1 + 1;
		else
			m_Index2 = m_Index1;

		// Factor by which to blend between the two sprites
		m_BlendFactor = LifeProgress - m_Index1;
	}
}