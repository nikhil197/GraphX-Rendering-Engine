#include "pch.h"
#include "CubeMap.h"
#include "GL/glew.h"

#include "stb/stb_image.h"

namespace GraphX
{
	CubeMap::CubeMap(const std::string& FilePath, const std::vector<std::string>& FileNames)
		: RendererResource(), m_FilePath(FilePath), m_FileNames(FileNames)
	{
		GX_PROFILE_FUNCTION()
		GX_ENGINE_INFO("Loading CubeMap: {0}", FilePath);

		// To flip the textures on load
		stbi_set_flip_vertically_on_load(0);

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		// Specify the parameters for texture wrapping
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		// Specify the parameters for texture filtering (min - max)
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_uc* localBuffer = nullptr;

		int channels;

		for (unsigned int i = 0; i < m_FileNames.size(); i++)
		{
			{
				GX_PROFILE_SCOPE("CubeMap::LoadTexFile")

				localBuffer = stbi_load((m_FilePath + m_FileNames[i]).c_str(), &m_Width, &m_Height, &channels, 0);
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

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, localBuffer);

			// Free the texture data
			stbi_image_free(localBuffer);
		}

		UnBind();
	}

	void CubeMap::InitRHI()
	{

	}

	void CubeMap::ReleaseRHI()
	{

	}

	void CubeMap::Bind(unsigned int slot) const
	{
		GX_PROFILE_FUNCTION()

		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
	}

	void CubeMap::UnBind() const
	{
		GX_PROFILE_FUNCTION()

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	CubeMap::~CubeMap()
	{
		GX_PROFILE_FUNCTION()

		glDeleteTextures(1, &m_RendererID);
	}
}