#include "pch.h"

#include "Mesh2D.h"
#include "VertexArray.h"
#include "Textures/Texture.h"
#include "Vertex.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexBufferLayout.h"
#include "Buffers/IndexBuffer.h"
#include "Shaders/Shader.h"

namespace GraphX
{
	Mesh2D::Mesh2D(const GM::Vector3& Pos, const GM::Vector3& Rotation, const GM::Vector2& Scale, const std::vector<Vertex2D>& Vertices, const std::vector<unsigned int>& Indices, Material* Mat)
		: Position(Pos), Rotation(Rotation), Scale(Scale), bShowDetails(0), m_Material(Mat), m_Vertices(Vertices), m_Indices(Indices), m_Model(GM::Matrix4()), m_UpdateModelMatrix(true)
	{
		m_VAO = new VertexArray();
		m_VBO = new VertexBuffer(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex2D));
		
		const VertexBufferLayout& layout = Vertex2D::VertexLayout();
		m_VAO->AddVertexBuffer(*m_VBO, layout);

		m_IBO = new IndexBuffer(&m_Indices[0], m_Indices.size());
		m_VAO->AddIndexBuffer(*m_IBO);
	}

	Mesh2D::Mesh2D(const Mesh2D& Mesh)
		: Position(Mesh.Position), Rotation(Mesh.Rotation), Scale(Mesh.Scale), bShowDetails(Mesh.bShowDetails), m_VAO(nullptr), m_VBO(new VertexBuffer(*Mesh.m_VBO)), m_IBO(new IndexBuffer(*Mesh.m_IBO)), m_Material(Mesh.m_Material), m_Vertices(Mesh.m_Vertices), m_Indices(Mesh.m_Indices), m_Model(Mesh.m_Model), m_UpdateModelMatrix(Mesh.m_UpdateModelMatrix)
	{
		const VertexBufferLayout& layout = Vertex2D::VertexLayout();
		m_VAO = new VertexArray();
		m_VAO->AddVertexBuffer(*m_VBO, layout);
		m_VAO->AddIndexBuffer(*m_IBO);
	}

	void Mesh2D::Update(float DeltaTime)
	{
		// Update the model matrix
		GM::Translation translation(Position);
		GM::Rotation rotation(Rotation);
		GM::Scaling scale(GM::Vector3(Scale, 1.0f));

		m_Model = translation * rotation * scale;
	}

	void Mesh2D::Enable() const
	{
		m_VAO->Bind();
	}

	void Mesh2D::Disable() const
	{
		m_VAO->UnBind();
	}

	void Mesh2D::BindBuffers() const
	{
		m_VAO->Bind();
	}

	void Mesh2D::UnBindBuffers() const
	{
		m_VAO->UnBind();
	}

	Mesh2D::~Mesh2D()
	{
		if (m_VBO != nullptr)
		{
			delete m_VBO;
			m_VAO = nullptr;
		}

		if (m_IBO != nullptr)
		{
			delete m_IBO;
			m_IBO = nullptr;
		}

		if (m_VAO != nullptr)
		{
			delete m_VAO;
			m_VAO = nullptr;
		}
	}
}