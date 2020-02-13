#include "pch.h"
#include "Texture.h"
#include "GL/glew.h"

#include "stb/stb_image.h"
#include "Utilities/EngineUtil.h"

namespace GraphX
{
	Texture::Texture(const std::string& filePath, bool TileTexture, unsigned int RowsInTexAtlas)
		: m_RendererID(0), m_FilePath(filePath), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0), m_TileTexture(TileTexture), m_RowsInTexAtlas(RowsInTexAtlas)
	{
		// To flip the texture
		stbi_set_flip_vertically_on_load(0);

		m_LocalBuffer = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_BPP, 4/*RGBA*/);

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		// Specify the parameters for texture wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_TileTexture ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_TileTexture ? GL_REPEAT : GL_CLAMP_TO_EDGE);

		// Specify the parameters for texture filtering (min - max)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
		UnBind();
	}

	Texture::Texture(int width, int height, FramebufferAttachmentType texType)
		: m_RendererID(0), m_FilePath(std::string()), m_LocalBuffer(nullptr), m_Width(width), m_Height(height), m_BPP(0), m_TileTexture(false), m_RowsInTexAtlas(1)
	{
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		if (texType == FramebufferAttachmentType::GX_TEX_COLOR)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		}
		else if (texType == FramebufferAttachmentType::GX_TEX_DEPTH)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		UnBind();
	}

	void Texture::Bind(unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void Texture::UnBind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	bool Texture::operator==(const Texture& OtherTex) const
	{
		return EngineUtil::ExtractFileName(OtherTex.GetFilePath()) == EngineUtil::ExtractFileName(m_FilePath) && OtherTex.m_Width == m_Width && OtherTex.m_Height == m_Height;
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_RendererID);

		// Free the local image data
		if (m_LocalBuffer)
			stbi_image_free(m_LocalBuffer);
	}

	bool operator==(const std::reference_wrapper<Texture>& Ref1, const std::reference_wrapper<Texture>& Ref2)
	{
		return Ref1.get().operator==(Ref2.get());
	}
}