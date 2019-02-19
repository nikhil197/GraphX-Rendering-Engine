#pragma once

namespace engine
{
	class Texture
	{
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

		/* Whether the texture will be used for tilling or not */
		bool m_TileTexture;

	public:
		/* Constructor */
		/* @Param TileTexture - Whether the texture will be used for tilling or not */
		Texture(const std::string& filePath, bool TileTexture = false);

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

		inline bool IsTileTexture() const { return m_TileTexture; }

		/* Destroy the texture */
		~Texture();
	};
}