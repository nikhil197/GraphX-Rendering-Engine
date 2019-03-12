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

	Mesh3D::Mesh3D(const Mesh3D& Mesh)
		: Position(Mesh.Position), Rotation(Mesh.Rotation), Scale(Mesh.Scale), BaseColor(Mesh.BaseColor), bShowDetails(Mesh.bShowDetails), Reflectivity(Mesh.Reflectivity), Shininess(Mesh.Shininess), m_Shader(Mesh.GetShader()), m_Textures(Mesh.GetTextures()) 
	{

	}

	void Mesh3D::Update(float DeltaTime)
	{
		gm::Translation translation(Position);
		gm::Rotation rotation(Rotation);
		gm::Scaling scale(Scale);

		m_Model = translation * rotation * scale;
	}

	void Mesh3D::Enable() const
	{
		BindBuffers();

		m_Shader.Bind();
		if(Reflectivity > 0)
			m_Shader.SetUniform1f("u_Reflectivity", Reflectivity);
		if(Shininess > 0)
			m_Shader.SetUniform1f("u_Shininess", Shininess);

		// Set the base color of the object
		if(BaseColor != gm::Vector4::ZeroVector)
			m_Shader.SetUniform4f("u_Color", BaseColor);

		// Bind the textures
		int NumTex = m_Textures.size();
		for (int i = 0; i < NumTex; i++)
		{
			const Texture* texture = m_Textures[i];
			texture->Bind(i);
			m_Shader.SetUniform1i((std::string("u_Texture") + std::to_string(i)).c_str(), i);
		}
	}

	void Mesh3D::Disable() const
	{
		UnBindBuffers();

		// UnBind the textures
		int NumTex = m_Textures.size();
		for (int i = 0; i < NumTex; i++)
		{
			const Texture* texture = m_Textures[i];
			texture->UnBind();
		}
	}

	void Mesh3D::BindBuffers() const
	{
		m_VAO->Bind();
		m_IBO->Bind();
	}

	void Mesh3D::UnBindBuffers() const
	{
		m_VAO->UnBind();
		m_IBO->UnBind();
	}

	void Mesh3D::AddTexture(const Texture* texture)
	{
		if (texture != nullptr)
		{
			m_Textures.push_back(texture);
		}
	}

	Mesh3D::~Mesh3D()
	{
		delete m_VBO;
		delete m_IBO;
		delete m_VAO;
	}
}