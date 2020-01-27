#pragma once

namespace GraphX
{
	class Material;

	class Mesh2D
	{
	public:
		/**
		@param Pos Position of the mesh in the world
		@param Rotation Rotation of the mesh in the world
		@param Scale Scale of the mesh
		@param Vertices vertices of the mesh (counter clockwise order)
		@param Indices indices into the vertices vector
		@param Mat Material used to render the mesh
		*/
		Mesh2D(const GM::Vector3& Pos, const GM::Vector3& Rotation, const GM::Vector2& Scale, const std::vector<struct Vertex2D>& Vertices, const std::vector<unsigned int>& Indices, Material* Mat = nullptr);

		Mesh2D(const Mesh2D& Other);

		/* Updates the status of the Mesh */
		virtual void Update(float DeltaTime);

		/* Prepares the object to be rendered */
		virtual void Enable() const;

		/* Unbinds all the buffers for the object */
		virtual void Disable() const;

		/* Binds the vertex buffer and the index buffer of the mesh */
		virtual void BindBuffers() const;

		/* UnBinds the vertex buffer and the index buffer of the mesh */
		virtual void UnBindBuffers() const;

		/* Returns the vao for the object */
		inline const class VertexArray* GetVAO() const { return m_VAO; }

		/* Returns the ibo for the object */
		inline const class IndexBuffer* GetIBO() const { return m_IBO; }

		/* Returns the vbo for the object */
		inline const class VertexBuffer* GetVBO() const { return m_VBO; }

		/* Returns the shader for the object */
		inline class Material* GetMaterial() const { return m_Material; }

		/* Sets new shader for the mesh */
		inline void SetMaterial(Material* NewMat) { m_Material = NewMat; }

		/* Returns the vertices of the object */
		inline const std::vector<Vertex2D>& GetVertices() const { return m_Vertices; }

		/* Returns the indices of the object */
		inline const std::vector<unsigned int>& GetIndices() const { return m_Indices; }

		/* Returns the model matrix for the mesh */
		inline const GM::Matrix4& GetModelMatrix() const { return m_Model; }

		virtual ~Mesh2D();

	public:
		/* Position of the object (3rd coordinate is need to for determining which object will be in front of which) */
		GM::Vector3 Position;

		/* Rotation of the object */
		GM::Vector3 Rotation;

		/* Scale of the object */
		GM::Vector2 Scale;

		/* Whether to show the details UI window (ImGUI) or not */
		bool bShowDetails : 1;

	protected:
		/* Vertex Array Object for the Mesh */
		class VertexArray* m_VAO;

		/* Vertex Buffer for the object */
		class VertexBuffer* m_VBO;

		/* Index Buffer for the Mesh */
		class IndexBuffer* m_IBO;

		/* Material used to render mesh */
		Material* m_Material;

		/* Vertex data */
		std::vector<struct Vertex2D> m_Vertices;

		/* Indices into the vertex buffer */
		std::vector<unsigned int> m_Indices;

		/* Model matrix for the mesh */
		GM::Matrix4 m_Model;

		/* Whether the mesh needs to updated or not */
		bool m_UpdateModelMatrix;
	};
}