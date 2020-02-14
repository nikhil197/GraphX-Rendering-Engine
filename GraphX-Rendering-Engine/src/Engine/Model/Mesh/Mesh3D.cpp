#include "pch.h"

#include "Mesh3D.h"
#include "VertexArray.h"
#include "Textures/Texture2D.h"
#include "Vertex.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexBufferLayout.h"
#include "Buffers/IndexBuffer.h"
#include "Materials/Material.h"

namespace GraphX
{
	Mesh3D::Mesh3D(const GM::Vector3& Pos, const GM::Vector3& Rotation, const GM::Vector3& Scale, const std::vector<Vertex3D>& Vertices, const std::vector<unsigned int>& Indices, const Ref<Material>& Mat)
		: Position(Pos), Rotation(Rotation), Scale(Scale), bShowDetails(0), m_Vertices(Vertices), m_Indices(Indices), m_Material(Mat), m_Model(), m_UpdateModelMatrix(true)
	{
		m_VAO = CreateRef<VertexArray>();
		m_VBO = CreateRef<VertexBuffer>(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex3D));
		const VertexBufferLayout& layout = Vertex3D::VertexLayout();
		m_VAO->AddVertexBuffer(*m_VBO, layout);

		m_IBO = CreateRef<IndexBuffer>(&m_Indices[0], m_Indices.size());
		m_VAO->AddIndexBuffer(*m_IBO);

		std::vector<GM::Vector3> positions;
		for (unsigned int i = 0; i < m_Vertices.size(); i++)
		{
			positions.push_back(m_Vertices[i].Position);
		}

		m_BoundingBox = CreateRef<GM::BoundingBox>(positions);
	}

	Mesh3D::Mesh3D(const Mesh3D& Mesh)
		: Position(Mesh.Position), Rotation(Mesh.Rotation), Scale(Mesh.Scale), bShowDetails(Mesh.bShowDetails), m_VAO(nullptr), m_VBO(new VertexBuffer(*Mesh.m_VBO)), m_IBO(new IndexBuffer(*Mesh.m_IBO)), m_Vertices(Mesh.m_Vertices), m_Indices(Mesh.m_Indices), m_Material(Mesh.m_Material), m_Model(Mesh.m_Model), m_UpdateModelMatrix(Mesh.m_UpdateModelMatrix)
	{
		const VertexBufferLayout& layout = Vertex3D::VertexLayout();
		m_VAO = CreateRef<VertexArray>();
		m_VAO->AddVertexBuffer(*m_VBO, layout);
		m_VAO->AddIndexBuffer(*m_IBO);

		std::vector<GM::Vector3> positions;
		for (unsigned int i = 0; i < m_Vertices.size(); i++)
		{
			positions.push_back(m_Vertices[i].Position);
		}

		m_BoundingBox = CreateRef<GM::BoundingBox>(positions);
	}

	void Mesh3D::Update(float DeltaTime)
	{
		if (m_UpdateModelMatrix)
		{
			GM::Translation translation(Position);
			GM::Rotation rotation(Rotation);
			GM::Scaling scale(Scale);

			m_Model = translation * rotation * scale;

			m_UpdateModelMatrix = false;
		}
	}

	void Mesh3D::Enable() const
	{
		m_VAO->Bind();
	}

	void Mesh3D::Disable() const
	{
		m_VAO->UnBind();
	}

	void Mesh3D::BindBuffers() const
	{
		m_VAO->Bind();
	}

	void Mesh3D::UnBindBuffers() const
	{
		m_VAO->UnBind();
	}

	Mesh3D::~Mesh3D()
	{
	}
}