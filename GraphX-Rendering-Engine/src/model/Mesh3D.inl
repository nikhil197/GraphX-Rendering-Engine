#include "VertexArray.h"
#include "Texture.h"
#include "Vertex.h"
#include "buffers/VertexBuffer.h"
#include "buffers/VertexBufferLayout.h"
#include "buffers/IndexBuffer.h"

namespace engine
{
	template<typename T>
	Mesh3D<T>::Mesh3D(const gm::Vector3& Pos, const gm::Vector3& Rotation, const gm::Vector3& Scale, Texture* texture, Shader& shader, std::vector<T>& vertices, std::vector<unsigned int>& indices)
		: Position(Pos), Rotation(Rotation), Scale(Scale), m_Texture(texture), m_Shader(shader), m_Vertices(vertices), m_Indices(indices)
	{
		m_VAO = new VertexArray();
		m_VBO = new VertexBuffer(&m_Vertices[0], m_Vertices.size() * sizeof(T));
		VertexBufferLayout layout = T::GetVertexLayout();
		m_VAO->AddBuffer(*m_VBO, layout);

		m_IBO = new IndexBuffer(&m_Indices[0], m_Indices.size());
	}

	template<typename T>
	gm::Matrix4 Mesh3D<T>::GetTransformationMatrix() const
	{
		gm::Translation translation(Position);
		gm::Rotation rotation(Rotation);
		gm::Scaling scale(Scale);

		return translation * rotation * scale;
	}

	template<typename T>
	Mesh3D<T>::~Mesh3D()
	{
		delete m_VBO;
		delete m_IBO;
		delete m_VAO;
		// Texture will be deleted by the application as other objects may be using the same texture
	}
}