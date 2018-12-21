#pragma once

namespace engine
{
	class Mesh2D
	{
	public:
		/* Position of the object (3rd coordinate is need to for determining which object will be in front of which) */
		gm::Vector3 Position;

		/* Rotation of the object */
		gm::Vector3 Rotation;

		/* Scale of the object */
		gm::Vector2 Scale;

		/* Whether to show the details UI window (ImGUI) or not */
		bool bShowDetails : 1;

	protected:
		/* Vertex Array Object for the Mesh */
		class VertexArray* m_VAO;

		/* Vertex Buffer for the object */
		class VertexBuffer* m_VBO;

		/* Index Buffer for the Mesh */
		class IndexBuffer* m_IBO;

		/* Shader used to render the object */
		class Shader& m_Shader;

		/* Textures used by the mesh */
		std::vector<const class Texture*> m_Textures;

		/* Vertex data */
		std::vector<struct Vertex2D> m_Vertices;

		/* Indices into the vertex buffer */
		std::vector<unsigned int> m_Indices;

	public:
		/**
		@param Pos Position of the mesh in the world
		@param Rotation Rotation of the mesh in the world
		@param Scale Scale of the mesh
		@param Shader shader used for shading (rendering) the mesh
		@param Textures textures used for the mesh
		@param Vertices vertices of the mesh (counter clockwise order)
		@param Indices indices into the vertices vector
		*/
		Mesh2D(const gm::Vector3& Pos, const gm::Vector3& Rotation, const gm::Vector2& Scale, class Shader& shader, const std::vector<const class Texture*>& Textures, const std::vector<struct Vertex2D>& Vertices, const std::vector<unsigned int>& Indices);

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