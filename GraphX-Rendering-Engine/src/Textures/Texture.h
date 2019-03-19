#pragma once

namespace engine
{
	/* Type of the texture used for the frame buffer */
	enum FramebufferAttachmentType
	{
		GX_TEX_NONE = 0,	/* Normal Texture */
		GX_TEX_COLOR,		/* Texture is used as a color buffer for a framebuffer */
		GX_TEX_DEPTH		/* Texture is used as a depth buffer for a framebuffer */
	};

	class Texture
	{
		/* Required to access the rendererID for the texture to bind to the framebuffer */
		friend class FrameBuffer;

	private:
		/* ID For the texture */
		unsigned int m_RendererID;

		/* Path to the texture file */
		std::string m_FilePath;

		/* To store the pixel data obtained from stb_image */
		unsigned char* m_LocalBuffer;

		/* Width and height of the texture */
		int m_Width, m_Height;

		/* Bits Per Pixel */
		int m_BPP;

		/* Whether the texture will be used for tiling or not */
		bool m_TileTexture;

		/* Number of rows in texture atlas */
		unsigned int m_RowsInTexAtlas;

	public:
		/* Constructor */
		/* @Param TileTexture - Whether the texture will be used for tilling or not */
		Texture(const std::string& filePath, bool TileTexture = false, unsigned int RowsInTexAtlas = 1);

		/* Constructor for the framebuffer textures */
		Texture(int width, int height, FramebufferAttachmentType texType);

		/* Bind the texture to the indicated slot */
		void Bind(unsigned int slot = 0) const;

		/* Unbind the currently bound texture */
		void UnBind() const;

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

		/* Destroy the texture */
		~Texture();
	};
}