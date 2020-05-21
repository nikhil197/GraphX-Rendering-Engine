#include "pch.h"
#include "Texture2D.h"
#include "GL/glew.h"

#include "stb/stb_image.h"
#include "Utilities/EngineUtil.h"

namespace GraphX
{
	Texture2D::Texture2D(const std::string& filePath, bool TileTexture, unsigned int RowsInTexAtlas)
		: RendererAsset(), m_FilePath(filePath), m_Width(0), m_Height(0), m_InternalFormat(0), m_DataFormat(0), m_TileTexture(TileTexture), m_RowsInTexAtlas(RowsInTexAtlas)
	{
		GX_PROFILE_FUNCTION()
		GX_ENGINE_INFO("Loading Texture2D: {0}", filePath);

		// To flip the texture
		stbi_set_flip_vertically_on_load(0);

		int channels;
		stbi_uc* localBuffer = nullptr;
		{
			GX_PROFILE_SCOPE("Texture2D::LoadTexFile")

			localBuffer = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &channels, 0);	// No desired channels 
			GX_ENGINE_ASSERT(localBuffer, "Failed to load texture data!");
		}

		if (channels == 4)
		{
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
		}

		GX_ENGINE_ASSERT(m_InternalFormat & m_DataFormat, " Texture Format not supported!");

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		// Specify the parameters for texture wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_TileTexture ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_TileTexture ? GL_REPEAT : GL_CLAMP_TO_EDGE);

		// Specify the parameters for texture filtering (min - max)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, localBuffer);
		UnBind();

		// Free the local image data
		if (localBuffer)
			stbi_image_free(localBuffer);
	}

	Texture2D::Texture2D(int width, int height, FramebufferAttachmentType texType)
		: RendererAsset(), m_FilePath(std::string()), m_Width(width), m_Height(height), m_InternalFormat(0), m_DataFormat(0), m_TileTexture(false), m_RowsInTexAtlas(1)
	{
		GX_PROFILE_FUNCTION()

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		if (texType == FramebufferAttachmentType::GX_TEX_COLOR)
		{
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, NULL);
		}
		else if (texType == FramebufferAttachmentType::GX_TEX_DEPTH)
		{
			m_InternalFormat = m_DataFormat = GL_DEPTH_COMPONENT;

			glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		UnBind();
	}

	Texture2D::Texture2D(int width, int height)
		: RendererAsset(), m_FilePath(std::string()), m_Width(width), m_Height(height), m_TileTexture(false), m_RowsInTexAtlas(1)
	{
		GX_PROFILE_FUNCTION()

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, width, height, 0, m_DataFormat, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	void Texture2D::Bind(unsigned int slot) const
	{
		GX_PROFILE_FUNCTION()

		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void Texture2D::UnBind() const
	{
		GX_PROFILE_FUNCTION()

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture2D::SetData(void* data, uint32_t size)
	{
		GX_PROFILE_FUNCTION()

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		GX_ENGINE_ASSERT(size == bpp * m_Width * m_Height, "Data must be for entire texture!");

		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	bool Texture2D::operator==(const Texture2D& OtherTex) const
	{
		return EngineUtil::ExtractFileName(OtherTex.GetFilePath()) == EngineUtil::ExtractFileName(m_FilePath) && OtherTex.m_Width == m_Width && OtherTex.m_Height == m_Height;
	}

	Texture2D::~Texture2D()
	{
		GX_PROFILE_FUNCTION()

		glDeleteTextures(1, &m_RendererID);
	}

	bool operator==(const std::reference_wrapper<Texture2D>& Ref1, const std::reference_wrapper<Texture2D>& Ref2)
	{
		return Ref1.get().operator==(Ref2.get());
	}
}