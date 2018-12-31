#include "pch.h"

#include "Mesh3D.h"
#include "VertexArray.h"
#include "Textures/Texture.h"
#include "Vertex.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexBufferLayout.h"
#include "Buffers/IndexBuffer.h"
#include "Shaders/Shader.h"

namespace engine
{
	Mesh3D::Mesh3D(const gm::Vector3& Pos, const gm::Vector3& Rotation, const gm::Vector3& Scale, Shader& shader, const std::vector<const Texture*>& Textures, const std::vector<Vertex3D>& Vertices, const std::vector<unsigned int>& Indices, const gm::Vector4& Color, float Reflect, float Shine)
		: Position(Pos), Rotation(Rotation), Scale(Scale), BaseColor(Color), bShowDetails(0), Reflectivity(Reflect), Shininess(Shine), m_Shader(shader), m_Textures(Textures), m_Vertices(Vertices), m_Indices(Indices)
	{
		m_VAO = new VertexArray();
		m_VBO = new VertexBuffer(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex3D));
		VertexBufferLayout layout = Vertex3D::GetVertexLayout();
		m_VAO->AddBuffer(*m_VBO, layout);

		m_IBO = new IndexBuffer(&m_Indices[0], m_Indices.size());
	}

	gm::Matrix4 Mesh3D::GetTransformationMatrix() const
	{
		gm::Translation translation(Position);
		gm::Rotation rotation(Rotation);
		gm::Scaling scale(Scale);

		return translation * rotation * scale;
	}

	void Mesh3D::Enable() const
	{
		m_VAO->Bind();
		m_IBO->Bind();

		m_Shader.Bind();
		m_Shader.SetUniform1f("u_Reflectivity", Reflectivity);
		m_Shader.SetUniform1f("u_Shininess", Shininess);

		// Set the base color of the object
		m_Shader.SetUniform4f("u_Color", BaseColor);

		// Bind the textures
		int NumTex = m_Textures.size();
		for (int i = 0; i < NumTex; i++)
		{
			const Texture* texture = m_Textures[i];
			texture->Bind(i);
			m_Shader.SetUniform1i("u_Texture" + i, i);
		}
	}

	void Mesh3D::Disable() const
	{
		m_VAO->UnBind();
		m_IBO->UnBind();

		// UnBind the textures
		int NumTex = m_Textures.size();
		for (int i = 0; i < NumTex; i++)
		{
			const Texture* texture = m_Textures[i];
			texture->UnBind();
		}
	}

	Mesh3D::~Mesh3D()
	{
		delete m_VBO;
		delete m_IBO;
		delete m_VAO;
	}
}