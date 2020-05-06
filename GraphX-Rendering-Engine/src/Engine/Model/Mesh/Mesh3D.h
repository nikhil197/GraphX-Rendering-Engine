#pragma once

namespace GraphX
{
	class Material;
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;

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
		Mesh3D(const GM::Vector3& Pos, const GM::Vector3& RotationMatrix, const GM::Vector3& Scale, const std::vector<struct Vertex3D>& Vertices, const std::vector<unsigned int>& Indices, const Ref<Material>& Mat = nullptr);

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
		inline Ref<const VertexArray> GetVAO() const { return m_VAO; }

		/* Returns the ibo for the object */
		inline Ref<const IndexBuffer> GetIBO() const { return m_IBO; }

		/* Returns the vbo for the object */
		inline Ref<const VertexBuffer> GetVBO() const { return m_VBO; }

		/* Returns the material used by the mesh */
		inline Ref<Material> GetMaterial() const { return m_Material; }

		/* Sets a new material for the mesh */
		inline void SetMaterial(const Ref<Material>& NewMat) { m_Material = NewMat; }

		/* Returns the model matrix for the mesh */
		inline const GM::Matrix4& GetModelMatrix() const { return m_Model; }

		/* Returns the original bounds of the mesh */
		inline const GM::BoxBounds& GetBounds() const { return m_Bounds; }
		
		/* Returns the bounding collision box */
		inline const Ref<struct GM::BoundingBox>& GetBoundingBox() const { return m_BoundingBox; }

		/* Sets new state for updating the model matrix */
		inline void UpdateModelMatrix(bool bCalculateMatrix) { m_UpdateModelMatrix = bCalculateMatrix; }

		virtual ~Mesh3D();

	public:
		/* Position of the object */
		GM::Vector3 Position;

		/* Rotation (Euler Angles) of the object about each axis */
		GM::Vector3 RotationMatrix;

		/* Scale of the object */
		GM::Vector3 Scale;

		/* Whether to show the details UI window (ImGUI) or not */
		bool bShowDetails : 1;

	protected:
		/* Vertex Array Object for the Mesh */
		Ref<VertexArray> m_VAO;

		/* Vertex Buffer for the object */
		Ref<VertexBuffer> m_VBO;

		/* Index Buffer for the Mesh */
		Ref<IndexBuffer> m_IBO;

		/* Material used to render the mesh */
		Ref<Material> m_Material;

		/* Vertex data */
		std::vector<struct Vertex3D> m_Vertices;

		/* Indices into the vertex buffer */
		std::vector<unsigned int> m_Indices;

		/* Model matrix for the mesh */
		GM::Matrix4 m_Model;

		/* Bounds of the box */
		GM::BoxBounds m_Bounds;

		/* AABB containing the whole object (independent of transformation) */
		Ref<GM::BoundingBox> m_BoundingBox;

		/* Whether the mesh needs to updated or not */
		bool m_UpdateModelMatrix;
	};
}