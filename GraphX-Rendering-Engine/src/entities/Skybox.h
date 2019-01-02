#pragma once

namespace engine
{
	class Skybox
	{
	private:
		/* Vertex Array for the skybox */
		class VertexArray* m_VAO;

		/* Vertex Buffer for the skybox */
		class VertexBuffer* m_VBO;

		/* Index Buffer for the skybox */
		class IndexBuffer* m_IBO;

		/* Shader for the skybox */
		class Shader* m_Shader;

		/* Cubemap used to render the skybox */
		class CubeMap* m_CubeMap;

		/* Reference to the player camera */
		const class Camera& m_Camera;

	public:
		/*
		*	Creates a skybox
		* @param ShaderFilePath Path to the file containing the shader code for the skybox
		* @param FilePath Path to the folder containing the textures to be used to draw the skybox
		* @param FileNames names of the files to be used as textures for the six faces of the skybox. Order of the names should be right, left, top, bottom, front, back faces
		* @param Camera Reference to the player camera
		*/
		Skybox(const std::string& ShaderFilePath, const std::string& FilePath, const std::vector<std::string>& FileNames, const class Camera& Camera);

		/* Prepare the skybox to be rendered */
		void Enable() const;

		/* Unbind the skybox buffers and shader to */
		void Disable() const;

		/* Returns the index buffer of the skybox */
		const class IndexBuffer& GetIBO() const { return *m_IBO; }

		~Skybox();
	};
}