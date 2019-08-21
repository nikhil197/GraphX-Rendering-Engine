#include "pch.h"

#include "CubeMap.h"
#include "stb/stb_image.h"

namespace GraphX
{
	CubeMap::CubeMap(const std::string& FilePath, const std::vector<std::string>& FileNames)
		: m_FilePath(FilePath), m_FileNames(FileNames), m_LocalBuffer(nullptr)
	{
		// To flip the textures on load
		stbi_set_flip_vertically_on_load(0);

		GLCall(glGenTextures(1, &m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));

		// Specify the parameters for texture wrapping
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

		// Specify the parameters for texture filtering (min - max)
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		for (unsigned int i = 0; i < m_FileNames.size(); i++)
		{
			m_LocalBuffer = stbi_load((m_FilePath + m_FileNames[i]).c_str(), &m_Width, &m_Height, &m_BPP, 4 /*RGBA*/);

			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));

			// Free the texture data
			stbi_image_free(m_LocalBuffer);
		}

		UnBind();
	}

	void CubeMap::Bind(unsigned int slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));
	}

	void CubeMap::UnBind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
	}

	CubeMap::~CubeMap()
	{
		GLCall(glDeleteTextures(1, &m_RendererID));
	}
}