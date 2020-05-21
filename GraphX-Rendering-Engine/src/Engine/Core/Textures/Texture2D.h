#pragma once

#include "Engine/Core/RendererAsset.h"

namespace GraphX
{
	/* Type of the texture used for the frame buffer */
	enum class FramebufferAttachmentType
	{
		GX_TEX_NONE	,		/* Normal Texture */
		GX_TEX_COLOR,		/* Texture is used as a color buffer for a framebuffer */
		GX_TEX_DEPTH		/* Texture is used as a depth buffer for a framebuffer */
	};

	class Texture2D
		: public RendererAsset
	{
		/* Required to access the rendererID for the texture to bind to the framebuffer */
		friend class FrameBuffer;

	public:
		/* Constructor */
		/* @Param TileTexture - Whether the texture will be used for tilling or not */
		Texture2D(const std::string& filePath, bool TileTexture = false, unsigned int RowsInTexAtlas = 1);

		/* Constructor for the framebuffer textures */
		Texture2D(int width, int height, FramebufferAttachmentType texType);

		/* Constructor for creating texture by code (Data needs to be set manually) */
		Texture2D(int width, int height);

		/* Bind the texture to the indicated slot */
		void Bind(unsigned int slot = 0) const;

		/* Unbind the currently bound texture */
		void UnBind() const;

		// Set Data of the texture
		void SetData(void* data, uint32_t size);

		/* Returns the width of the texture */
		inline int GetWidth() const { return m_Width; }

		/* Returns the height of the texture */
		inline int GetHeight() const { return m_Height; }

		/* Returns the file path of the texture */
		inline const std::string& GetFilePath() const { return m_FilePath; }

		/* Returns whether the texture will be used for tiling */
		inline bool IsTileTexture() const { return m_TileTexture; }

		/* Returns the number of rows in the texture atlas */
		inline unsigned int GetRowsInTexAtlas() const { return m_RowsInTexAtlas; }

		/* Equality Test for the texture */
		bool operator==(const Texture2D& OtherTex) const;

		/* Destroy the texture */
		~Texture2D();

	private:
		/* Path to the texture file */
		std::string m_FilePath;

		/* Width and height of the texture */
		int m_Width, m_Height;

		/* Format of the texture storage and the supplied data */
		uint32_t m_InternalFormat, m_DataFormat;

		/* Whether the texture will be used for tiling or not */
		bool m_TileTexture;

		/* Number of rows in texture atlas */
		uint32_t m_RowsInTexAtlas;
	};

	/* Equality test for a Texture wrapped in reference_wrapper */
	bool operator==(const std::reference_wrapper<Texture2D>& Ref1, const std::reference_wrapper<Texture2D>& Ref2);
}