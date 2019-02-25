#include "pch.h"
#include "Texture.h"

#include "stb/stb_image.h"

namespace engine
{
	Texture::Texture(const std::string& filePath, bool TileTexture)
		: m_RendererID(0), m_FilePath(filePath), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0), m_TileTexture(TileTexture)
	{
		// To flip the texture
		stbi_set_flip_vertically_on_load(1);

		m_LocalBuffer = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_BPP, 4/*RGBA*/);

		GLCall(glGenTextures(1, &m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

		// Specify the parameters for texture wrapping
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_TileTexture ? GL_REPEAT : GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_TileTexture ? GL_REPEAT : GL_CLAMP_TO_EDGE));

		// Specify the parameters for texture filtering (min - max)
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer))
		UnBind();
	}

	Texture::Texture(int width, int height, FramebufferAttachmentType texType)
		: m_RendererID(0), m_FilePath(std::string()), m_LocalBuffer(nullptr), m_Width(width), m_Height(height), m_BPP(0), m_TileTexture(false)
	{
		GLCall(glGenTextures(1, &m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

		if (texType == FramebufferAttachmentType::GX_TEX_COLOR)
		{
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
		}
		else if (texType == FramebufferAttachmentType::GX_TEX_DEPTH)
		{
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
		}
	}

	void Texture::Bind(unsigned int slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	}

	void Texture::UnBind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	Texture::~Texture()
	{
		GLCall(glDeleteTextures(1, &m_RendererID));

		// Free the local image data
		if (m_LocalBuffer)
			stbi_image_free(m_LocalBuffer);
	}
}