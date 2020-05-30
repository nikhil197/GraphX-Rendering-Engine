#include "pch.h"

#include "Mesh3D.h"
#include "VertexArray.h"
#include "Textures/Texture2D.h"
#include "Vertex.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexBufferLayout.h"
#include "Buffers/IndexBuffer.h"
#include "Materials/Material.h"

#include "Utilities/Importer.h"

namespace GraphX
{
	void Mesh3D::Load(const std::string& FilePath, const Ref<Material>& Mat, std::vector<Ref<Mesh3D>>& Meshes)
	{
		GX_ENGINE_INFO("Loading Model {0}", FilePath);
		GX_PROFILE_FUNCTION()

		std::vector<std::vector<Ref<const Texture2D>>> Textures;
		std::size_t InitialSize = Meshes.size() == 0 ? 0 : Meshes.size() - 1;

		bool Loaded = false;
		{
			GX_PROFILE_SCOPE("Assimp-Load-Model")

			Loaded = Importer::Get()->ImportModel(FilePath, Meshes, Textures);
		}

		if (Loaded)
		{
			// Set the shader for each mesh
			unsigned int i = 0;
			std::vector<Ref<Mesh3D>>::iterator itr = Meshes.begin() + InitialSize;
			std::vector<Ref<Mesh3D>>::iterator end = Meshes.end();

			while (itr != end)
			{
				Ref<Material> NewMat = Mat;
				NewMat->AddTexture(Textures[i]);

				(*itr)->SetMaterial(NewMat);
				itr++;
				i++;
			}
		}
		else
		{
			GX_ENGINE_ERROR("Failed to load Model {0}", FilePath);
		}
	}

	Mesh3D::Mesh3D(const GM::Vector3& Pos, const GM::Rotator& Rotation, const GM::Vector3& Scale, const std::vector<Vertex3D>& Vertices, const std::vector<unsigned int>& Indices, const Ref<Material>& Mat)
		: Position(Pos), Rotation(Rotation), Scale(Scale), bShowDetails(0), m_Vertices(Vertices), m_Indices(Indices), m_Material(Mat), m_Model(), m_UpdateModelMatrix(true)
	{
		GX_PROFILE_FUNCTION()

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
		m_Bounds = GM::BoxBounds(positions);
	}

	Mesh3D::Mesh3D(const Mesh3D& Mesh)
		: Position(Mesh.Position), Rotation(Mesh.Rotation), Scale(Mesh.Scale), bShowDetails(Mesh.bShowDetails), m_VAO(nullptr), m_VBO(new VertexBuffer(*Mesh.m_VBO)), m_IBO(new IndexBuffer(*Mesh.m_IBO)), m_Vertices(Mesh.m_Vertices), m_Indices(Mesh.m_Indices), m_Material(Mesh.m_Material), m_Model(Mesh.m_Model), m_UpdateModelMatrix(Mesh.m_UpdateModelMatrix)
	{
		GX_PROFILE_FUNCTION()

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
		m_Bounds = GM::BoxBounds(positions);
	}

	void Mesh3D::Update(float DeltaTime)
	{
		if (m_UpdateModelMatrix)
		{
			GM::ScaleRotationTranslationMatrix::Make(m_Model, Scale, Rotation, Position);

			// Update the bounding box here, instead of during the rendering process
			m_BoundingBox->Transform(m_Bounds, m_Model);

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