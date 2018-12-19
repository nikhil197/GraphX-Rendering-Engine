#pragma once

#include "pch.h"

namespace engine
{
	template<typename T>
	class Mesh2D
	{
	public:
		/* Position of the object (3rd coordinate is need to for determining which object will be in front of which) */
		gm::Vector3 Position;

		/* Rotation of the object */
		gm::Vector3 Rotation;

		/* Scale of the object */
		gm::Vector2 Scale;

	private:
		/* Vertex Array Object for the Mesh */
		class VertexArray* m_VAO;

		/* Vertex Buffer for the object */
		class VertexBuffer* m_VBO;

		/* Index Buffer for the Mesh */
		class IndexBuffer* m_IBO;

		// Multiple textures can be used to texture the object
		// GX currently supports only single texture rendering
		/* Texture used by the mesh */
		const class Texture* m_Texture;

		/* Shader used to render the object */
		class Shader& m_Shader;

		/* Vertex data */
		std::vector<T>& m_Vertices;

		/* Indices into the vertex buffer */
		std::vector<unsigned int>& m_Indices;

	public:
		/* Constructor when using textures */
		Mesh2D(const gm::Vector3& Pos, const gm::Vector3& Rotation, const gm::Vector2& Scale, class Texture* texture, class Shader& shader, std::vector<T>& vertices, std::vector<unsigned int>& indices);

		/* Returns the vao for the object */
		inline class VertexArray* GetVAO() const { return m_VAO; }

		/* Returns the ibo for the object */
		inline class IndexBuffer* GetIBO() const { return m_IBO; }

		/* Returns the vbo for the object */
		inline class VertexBuffer* GetVBO() const { return m_VBO; }

		/* Returns a 4x4 transformation matrix for the object */
		gm::Matrix4 GetTransformationMatrix() const;

		~Mesh2D();

	};
}

// Implementation of the template
#include "Mesh2D.inl"