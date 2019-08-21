#pragma once

namespace GraphX
{
	class CubeMap
	{
	private:
		/* ID for the cubemap used by the renderer */
		unsigned int m_RendererID;

		/* Path to the cubemap texture */
		std::string m_FilePath;

		/* Names of the 6 textures located at the path specified in the file path */
		std::vector<std::string> m_FileNames;

		/* Local buffer to store the pixel data */
		unsigned char* m_LocalBuffer;

		/* Width and Height for each texture */
		int m_Width, m_Height;

		/* Bits per pixel for each texture */
		int m_BPP;

	public:
		/*
		*	Creates a cubemap
		* @param FilePath Path to the folder containing the textures to be used to draw the cubemap
		* @param FileNames names of the files to be used as textures for the six faces of the cubemap. Order of the names should be right, left, top, bottom, front, back faces
		*/
		CubeMap(const std::string& FilePath, const std::vector<std::string>& FileNames);

		/* Bind the cubemap to specified slot */
		void Bind(unsigned int slot = 0) const;

		/* Unbind the currenlty bound cubemap */
		void UnBind() const;

		/* Returns the width of the texture */
		inline int GetWidth() const { return m_Width; }

		/* Returns the height of the texture */
		inline int GetHeight() const { return m_Height; }

		~CubeMap();
	};
}