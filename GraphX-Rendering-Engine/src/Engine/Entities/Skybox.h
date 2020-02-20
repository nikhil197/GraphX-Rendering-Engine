#pragma once

#include "Entity.h"

namespace GraphX
{
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class Shader;
	class CubeMap;
	class CameraController;

	class Skybox
		: public Entity
	{
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
		*/
		Skybox(const std::string& ShaderFilePath, const std::string& FilePath, const std::vector<std::string>& FileNames, const GM::Matrix4& ViewMat, const GM::Vector4& color, float factor = 0.0f, unsigned int slot = 0, float Speed = 1.0f);

		void Update(float DeltaTime) override;

		/* Prepare the skybox to be rendered */
		void Enable() const;

		/* Unbind the skybox buffers and shader to */
		virtual void Disable() const override;

		/* Returns the index buffer of the skybox */
		inline Ref<IndexBuffer> GetIBO() const { return m_IBO; }

		/* Returns the skybox view */
		inline const GM::Matrix4& GetView() const { return m_View; }

		inline int GetBindingSlot() const { return m_BindingSlot; }

		inline const GM::Vector4& GetBlendColor() const { return m_BlendColor; }

		virtual ~Skybox();

	private:
		virtual void Enable(class Shader& shader, const std::string& Name = "") const override;

	private:
		/* Vertex Array for the skybox */
		Scope<VertexArray> m_VAO;

		/* Vertex Buffer for the skybox */
		Scope<VertexBuffer> m_VBO;

		/* Index Buffer for the skybox */
		Ref<IndexBuffer> m_IBO;

		/* Shader for the skybox */
		Ref<Shader> m_Shader;

		/* Cubemap used to render the skybox */
		Ref<CubeMap> m_CubeMap;

		/* Binding slot for the cubemap */
		unsigned int m_BindingSlot;

		/* Current Rotation of the skybox */
		float m_Rotation;

		/* View matrix required for the skybox */
		GM::Matrix4 m_View;

		/* Color to blend with */
		GM::Vector4 m_BlendColor;
	};
}