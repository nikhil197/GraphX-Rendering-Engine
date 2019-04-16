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
		: Position(Pos), Rotation(Rotation), Scale(Scale), TintColor(Color), bShowDetails(0), Reflectivity(Reflect), Shininess(Shine), m_Shader(shader), m_Textures(Textures), m_Vertices(Vertices), m_Indices(Indices), m_Model(), m_UpdateModelMatrix(true)
	{
		m_VAO = new VertexArray();
		m_VBO = new VertexBuffer(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex3D));
		VertexBufferLayout layout = Vertex3D::GetVertexLayout();
		m_VAO->AddBuffer(*m_VBO, layout);

		m_IBO = new IndexBuffer(&m_Indices[0], m_Indices.size());
	}

	Mesh3D::Mesh3D(const Mesh3D& Mesh)
		: Position(Mesh.Position), Rotation(Mesh.Rotation), Scale(Mesh.Scale), TintColor(Mesh.TintColor), bShowDetails(Mesh.bShowDetails), Reflectivity(Mesh.Reflectivity), Shininess(Mesh.Shininess), m_VAO(Mesh.m_VAO), m_VBO(Mesh.m_VBO), m_IBO(Mesh.m_IBO), m_Shader(Mesh.GetShader()), m_Textures(Mesh.GetTextures()), m_Vertices(Mesh.m_Vertices), m_Indices(Mesh.m_Indices), m_Model(Mesh.m_Model), m_UpdateModelMatrix(Mesh.m_UpdateModelMatrix)
	{
	}

	void Mesh3D::Update(float DeltaTime)
	{
		if (m_UpdateModelMatrix)
		{
			gm::Translation translation(Position);
			gm::Rotation rotation(Rotation);
			gm::Scaling scale(Scale);

			m_Model = translation * rotation * scale;

			m_UpdateModelMatrix = false;
		}
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
		m_Shader.SetUniform4f("u_TintColor", TintColor);
		
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