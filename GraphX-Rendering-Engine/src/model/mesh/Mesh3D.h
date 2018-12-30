#pragma once

namespace engine
{
	class Mesh3D
	{
	public:
		/* Position of the object */
		gm::Vector3 Position;

		/* Rotation (Euler Angles) of the object about each axis */
		gm::Vector3 Rotation;

		/* Scale of the object */
		gm::Vector3 Scale;

		/* Whether to show the details UI window (ImGUI) or not */
		bool bShowDetails : 1;

		/* Reflectivity of the object */
		float Reflectivity;

		/* Shininess of the object */
		float Shininess;

	private:
		/* Vertex Array Object for the Mesh */
		class VertexArray* m_VAO;

		/* Vertex Buffer for the object */
		class VertexBuffer* m_VBO;

		/* Index Buffer for the Mesh */
		class IndexBuffer* m_IBO;

		/* Shader used to render the object */
		class Shader& m_Shader;

		// Multiple textures can be used to texture the object
		// GX currently supports only single texture rendering
		/* Texture used by the mesh */
		std::vector<const class Texture*> m_Textures;

		/* Vertex data */
		std::vector<struct Vertex3D> m_Vertices;

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
		Mesh3D(const gm::Vector3& Pos, const gm::Vector3& Rotation, const gm::Vector3& Scale, class Shader& shader, const std::vector<const class Texture*> Textures, const std::vector<struct Vertex3D>& Vertices, const std::vector<unsigned int>& Indices, float Reflect = 0.5f, float Shine = 32.0f);

		/* Prepares the object to be rendered */
		void Enable() const;

		/* Unbinds all the buffers for the object */
		void Disable() const;

		/* Returns the vao for the object */
		inline const class VertexArray* GetVAO() const { return m_VAO; }

		/* Returns the ibo for the object */
		inline const class IndexBuffer* GetIBO() const { return m_IBO; }

		/* Returns the vbo for the object */
		inline const class VertexBuffer* GetVBO() const { return m_VBO; }

		/* Returns the shader for the object */
		inline class Shader& GetShader() const { return m_Shader; }

		/* Returns a 4x4 transformation matrix for the object */
		gm::Matrix4 GetTransformationMatrix() const;

		virtual ~Mesh3D();
	};
}