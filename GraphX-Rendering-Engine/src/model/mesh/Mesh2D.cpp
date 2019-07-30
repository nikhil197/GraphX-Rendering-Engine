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
	Mesh2D::Mesh2D(const GraphXMaths::Vector3& Pos, const GraphXMaths::Vector3& Rotation, const GraphXMaths::Vector2& Scale, Shader& shader, const std::vector<const Texture*>& Textures, const std::vector<Vertex2D>& Vertices, const std::vector<unsigned int>& Indices, const GraphXMaths::Vector4& Color, float Reflect, float Shine)
		: Position(Pos), Rotation(Rotation), Scale(Scale), TintColor(Color), bShowDetails(0), Reflectivity(Reflect), Shininess(Shine), m_Shader(shader), m_Textures(Textures), m_Vertices(Vertices), m_Indices(Indices)
	{
		m_VAO = new VertexArray();
		m_VBO = new VertexBuffer(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex2D));
		VertexBufferLayout layout = Vertex2D::GetVertexLayout();
		m_VAO->AddBuffer(*m_VBO, layout);

		m_IBO = new IndexBuffer(&m_Indices[0], m_Indices.size());
		m_VAO->AddIndexBuffer(*m_IBO);
	}

	void Mesh2D::Update(float DeltaTime)
	{
		// Update the model matrix
		GraphXMaths::Translation translation(Position);
		GraphXMaths::Rotation rotation(Rotation);
		GraphXMaths::Scaling scale(GraphXMaths::Vector3(Scale, 1.0f));

		m_Model = translation * rotation * scale;
	}

	void Mesh2D::Enable() const
	{
		m_VAO->Bind();

		m_Shader.Bind();
		if(Reflectivity > 0)
			m_Shader.SetUniform1f("u_Reflectivity", Reflectivity);
		if(Shininess > 0)
			m_Shader.SetUniform1f("u_Shininess", Shininess);

		// Set the base Color of the object
		if(TintColor != GraphXMaths::Vector4::ZeroVector)
			m_Shader.SetUniform4f("u_Color", TintColor);

		int NumTex = m_Textures.size();
		for (int i = 0; i < NumTex; i++)
		{
			const Texture* texture = m_Textures[i];
			texture->Bind(i);
			m_Shader.SetUniform1i("u_Texture" + i, i);
		}
	}

	void Mesh2D::Disable() const
	{
		m_VAO->UnBind();

		// Bind the textures
		int NumTex = m_Textures.size();
		for (int i = 0; i < NumTex; i++)
		{
			const Texture* texture = m_Textures[i];
			texture->UnBind();
		}
	}

	void Mesh2D::BindBuffers() const
	{
		m_VAO->Bind();
	}

	void Mesh2D::UnBindBuffers() const
	{
		m_VAO->UnBind();
	}

	void Mesh2D::AddTexture(const Texture* texture)
	{
		if (texture != nullptr)
		{
			m_Textures.push_back(texture);
		}
	}

	Mesh2D::~Mesh2D()
	{
		delete m_VBO;
		delete m_IBO;
		delete m_VAO;
	}
}