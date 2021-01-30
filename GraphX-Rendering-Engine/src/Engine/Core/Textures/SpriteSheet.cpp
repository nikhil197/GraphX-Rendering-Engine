#include "pch.h"
#include "SpriteSheet.h"

#include "SubTexture2D.h"

namespace GraphX
{
	SpriteSheet::SpriteSheet(const std::string& InFilePath, uint32_t InNumSprites, const GM::Vector2& InSpriteSize)
		: Texture2D(InFilePath), m_NumSprites(InNumSprites), m_SpriteSize(InSpriteSize)
	{
		m_NumSpritesInRow = m_Width / (uint32_t)m_SpriteSize.x;
		m_Rows = m_Height / (uint32_t)m_SpriteSize.y;

		m_Sprites.reserve(m_NumSprites);

		// Parent texture reference to set in the subtexture
		Ref<Texture2D> BaseTexture = CreateRef<SpriteSheet>(*this);
		
		GM::IntVector2 CellIndex;
		for (uint32_t i = 0; i < m_NumSprites; i++)
		{
			CellIndex.x = i % m_NumSpritesInRow;
			CellIndex.y = i / m_NumSpritesInRow;
			m_Sprites.emplace_back(SubTexture2D::Create(BaseTexture, CellIndex, InSpriteSize, GM::IntVector2::UnitVector, false));
		}
	}

	Ref<SubTexture2D> SpriteSheet::GetSprite(const uint32_t InSpriteIndex) const
	{
		GX_ENGINE_ASSERT(InSpriteIndex < m_Sprites.size(), "Invalid Sprite index!")

		return m_Sprites[InSpriteIndex];
	}
}