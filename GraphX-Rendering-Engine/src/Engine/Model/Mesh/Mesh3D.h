#pragma once

namespace GraphX
{
	class Material;
	
	/* 3D Representation of an object in the scene */
	class Mesh3D
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
		Mesh3D(const GM::Vector3& Pos, const GM::Vector3& Rotation, const GM::Vector3& Scale, const std::vector<struct Vertex3D>& Vertices, const std::vector<unsigned int>& Indices, Material* Mat = nullptr);

		// Copy Constructor
		Mesh3D(const Mesh3D& Mesh);

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

		/* Returns the material used by the mesh */
		inline class Material* GetMaterial() const { return m_Material; }

		/* Sets a new material for the mesh */
		inline void SetMaterial(Material* NewMat) { m_Material = NewMat; }

		/* Returns the model matrix for the mesh */
		inline const GM::Matrix4& GetModelMatrix() const { return m_Model; }

		/* Returns the bounding collision box */
		inline const struct GM::BoundingBox* GetBoundingBox() const { return m_BoundingBox; }

		/* Sets new state for updating the model matrix */
		inline void UpdateModelMatrix(bool bCalculateMatrix) { m_UpdateModelMatrix = bCalculateMatrix; }

		virtual ~Mesh3D();

	public:
		/* Position of the object */
		GM::Vector3 Position;

		/* Rotation (Euler Angles) of the object about each axis */
		GM::Vector3 Rotation;

		/* Scale of the object */
		GM::Vector3 Scale;

		/* Whether to show the details UI window (ImGUI) or not */
		bool bShowDetails : 1;

	protected:
		/* Vertex Array Object for the Mesh */
		class VertexArray* m_VAO;

		/* Vertex Buffer for the object */
		class VertexBuffer* m_VBO;

		/* Index Buffer for the Mesh */
		class IndexBuffer* m_IBO;

		/* Material used to render the mesh */
		Material* m_Material;

		/* Vertex data */
		std::vector<struct Vertex3D> m_Vertices;

		/* Indices into the vertex buffer */
		std::vector<unsigned int> m_Indices;

		/* Model matrix for the mesh */
		GM::Matrix4 m_Model;

		/* Box containing the whole object */
		GM::BoundingBox* m_BoundingBox;

		/* Whether the mesh needs to updated or not */
		bool m_UpdateModelMatrix;
	};
}