#pragma once

#include "Texture2D.h"

namespace GraphX
{
	/**
	* A Texture which comprises of multiple smaller sub-textures
	* 
	* This Sprite sheet class is supposed to be used for representing multiple sub textures of same size in a tight arrangement
	* (starting from top right) in the parent texture
	*/
	class SpriteSheet
		: public Texture2D
	{
	public:
		SpriteSheet(const std::string& InFilePath, uint32_t InNumSprites, const GM::Vector2& InSpriteSize);

		/* Returns the number of sprites in the sprite sheet */
		inline uint32_t GetNumSprites() const { return m_NumSprites; }

		/* Returns the number of sprites in a single row of the sprite sheet */
		inline uint32_t GetNumSpritesInRow() const { return m_NumSpritesInRow; }

		/* Returns the number of rows which contains sprites */
		// Note: Different from GetRowsInAtlas() which returns total number of rows in the sprite sheet.
		// # of rows containing sprites can be less than total rows
		inline uint32_t GetNumSpriteRows() const { return m_NumSprites / m_NumSpritesInRow; }

		/* Returns a sprite specified by the SpriteIndex */
		Ref<class SubTexture2D> GetSprite(const uint32_t InSpriteIndex) const;

		// Overrides
		virtual uint32_t GetRowsInTexAtlas() const override { return m_Rows; }
		virtual bool IsSpriteSheet() const override { return true; }

	private:
		/* Number of sprites in this sprite sheet */
		uint32_t m_NumSprites;

		/* Number of rows in the sprite */
		uint32_t m_Rows;

		/* Maximum number of sprites in a row */
		uint32_t m_NumSpritesInRow;

		/* Size of each sprite (sub texture) */
		GM::Vector2 m_SpriteSize;

		/* All the sub textures in the sprite sheet */
		std::vector<Ref<class SubTexture2D>> m_Sprites;
	};
}