#pragma once

namespace GraphX
{
	class Texture2D;

	class SubTexture2D
	{
	public:
		/*
		* Creates subtexture which is a part of a bigger texture (sprite sheets)
		* @param InTexture Bigger texture from which subtexture is extracted
		* @param Min Lower limits of the texture coords
		* @param Max Upper limits of the texture coords
		*/
		SubTexture2D(const Ref<Texture2D>& InTexture, const GM::Vector2& Min, const GM::Vector2& Max);

		/*
		* Creates subtexture which is a part of a bigger texture (sprite sheets)
		* @param InTexture Bigger texture from which subtexture is extracted
		* @param CellIndex x & y Index of the lowermost and leftmost cell of the subtexture in the InTexture
		* @param CellSize Size of each block of the texture
		* @param CellSpan No. of cells this subtexture spans in x & y direction
		* @param fromBottomLeft If true, indices start from bottom left else indices start from top left
		*/
		static Ref<SubTexture2D> Create(const Ref<Texture2D>& InTexture, const GM::IntVector2& CellIndex, const GM::Vector2& CellSize, const GM::IntVector2& CellSpan, bool FromBottomLeft = true);

		/* Returns the texture of which this subtexture is a part of */
		const Ref<Texture2D> GetTexture() const { return m_Texture; }

		/* Returns the texture coords that represent this subtexture within the given texture */
		const GM::Vector2* GetTexCoords() const { return m_TexCoords; }

	private:
		/* Texture of which this is a part of */
		Ref<Texture2D> m_Texture;

		/* Texture coords required for rendering this subtexture */
		GM::Vector2 m_TexCoords[4];
	};
}