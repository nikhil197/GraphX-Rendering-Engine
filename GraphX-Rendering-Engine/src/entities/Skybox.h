#pragma once

#include "Entity.h"

namespace GraphX
{
	class Skybox
		: public Entity
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

		/* Binding slot for the cubemap */
		unsigned int m_BindingSlot;

		/* Current Rotation of the skybox */
		float m_Rotation;

		/* View matrix required for the skybox */
		GraphXMaths::Matrix4 m_View;

		/* Color to blend with */
		GraphXMaths::Vector4 m_BlendColor;

	public:
		/* Speed at which the skybox will rotate */
		float RotationSpeed;

		/* Blend factor used to blend the cubemap and blend color */
		float BlendFactor;

	public:
		/*
		*	Creates a skybox
		* @param ShaderFilePath Path to the file containing the shader code for the skybox
		* @param FilePath Path to the folder containing the textures to be used to draw the skybox
		* @param FileNames names of the files to be used as textures for the six faces of the skybox. Order of the names should be right, left, top, bottom, front, back faces
		* @param Camera Reference to the player camera
		*/
		Skybox(const std::string& ShaderFilePath, const std::string& FilePath, const std::vector<std::string>& FileNames, const class Camera& Camera, const GraphXMaths::Vector4& color, float factor = 0.0f, unsigned int slot = 0, float Speed = 1.0f);

		void Update(float DeltaTime) override;

		/* Prepare the skybox to be rendered */
		void Enable() const;

		/* Unbind the skybox buffers and shader to */
		void Disable() const override;

		/* Returns the index buffer of the skybox */
		const class IndexBuffer& GetIBO() const { return *m_IBO; }

		~Skybox();

	private:
		void Enable(class Shader& shader, const std::string& Name) const override;
	};
}