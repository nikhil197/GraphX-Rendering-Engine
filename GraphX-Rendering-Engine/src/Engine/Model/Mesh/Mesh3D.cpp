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
	////////////////////////////////////
	// RawMeshData
	///////////////////////////////////
	RawMeshData::RawMeshData(const std::vector<Vertex3D>& verts, const std::vector<uint32_t>& indices)
		: Vertices(verts), Indices(indices), SectionInfos(std::vector<MeshSectionInfo>(1, MeshSectionInfo()))
	{}

	/* */
	RawMeshData::RawMeshData(const std::vector<Vertex3D>& verts, const std::vector<uint32_t>& indices, const std::vector<MeshSectionInfo>& sectionStartIndices)
		: Vertices(verts), Indices(indices), SectionInfos(sectionStartIndices)
	{}

	/////////////////////////////////////
	// MaterialToSectionMap
	////////////////////////////////////

	void MaterialToSectionMap::Add(const uint32_t MaterialIndex, const uint32_t SectionIndex)
	{
		auto itr = m_Map.find(MaterialIndex);
		if (itr != m_Map.end())
		{
			itr->second.emplace_back(SectionIndex);
		}
		else
		{
			m_Map.insert({ MaterialIndex, std::vector<uint32_t>(1, SectionIndex) });
		}
	}

	bool MaterialToSectionMap::Exists(const uint32_t MaterialIndex) const
	{
		return m_Map.find(MaterialIndex) != m_Map.end();
	}

	const std::vector<uint32_t>& MaterialToSectionMap::GetSections(const uint32_t MaterialIndex) const
	{
		auto itr = m_Map.find(MaterialIndex);

		GX_ENGINE_ASSERT(itr != m_Map.end(), "Material Not present in the map");
		return itr->second;
	}

	uint32_t MaterialToSectionMap::GetSection(const uint32_t MaterialIndex, const uint32_t SectionNum) const
	{
		auto itr = m_Map.find(MaterialIndex);

		GX_ENGINE_ASSERT(itr != m_Map.end(), "Material Not present in the map");
		GX_ENGINE_ASSERT(itr->second.size() >= SectionNum, "Number of section referencing this material are less than the requested number ");
		return itr->second.at(SectionNum - 1);
	}

	////////////////////////////////////
	// Mesh 3D
	///////////////////////////////////

	Ref<Mesh3D> Mesh3D::Load(const std::string& FilePath, const Ref<Material>& InMat)
	{
		GX_ENGINE_INFO("Loading Model {0}", FilePath);
		GX_PROFILE_FUNCTION()

			GX_ENGINE_ASSERT(InMat != nullptr, "Material cannot be nullptr");

		Ref<Mesh3D> ResultMesh = CreateRef<Mesh3D>(Vector3::ZeroVector, Rotator::ZeroRotator, Vector3::UnitVector);

		bool Loaded = false;
		{
			GX_PROFILE_SCOPE("Assimp-Load-Model")

			Loaded = Importer::Get()->ImportMesh(FilePath, ResultMesh.get(), InMat);
		}

		if (Loaded)
		{
			ResultMesh->BuildMesh();
		}
		else
		{
			GX_ENGINE_ERROR("Failed to load Model {0}", FilePath);
		}

		return ResultMesh;
	}

	Mesh3D::Mesh3D(const GM::Vector3& Pos, const GM::Rotator& Rotation, const GM::Vector3& Scale, RawMeshData* RawData, const Ref<Material>& Mat)
		: Position(Pos), Rotation(Rotation), Scale(Scale), bShowDetails(0), m_Model(), m_MaterialMap(), m_UpdateModelMatrix(true)
	{
		GX_PROFILE_FUNCTION()

		if (RawData != nullptr)
		{
			m_RawData = CreateRef<RawMeshData>(RawData);
			BuildMesh();
		}
		else
		{
			m_RawData = CreateRef<RawMeshData>();
		}

		if (Mat != nullptr)
		{
			m_Materials.emplace_back(Mat);
		}
	}

	Mesh3D::Mesh3D(const GM::Vector3& Pos, const GM::Rotator& Rotation, const GM::Vector3& Scale, const std::vector<Vertex3D>& Vertices, const std::vector<unsigned int>& Indices, const Ref<Material>& Mat)
		: Position(Pos), Rotation(Rotation), Scale(Scale), bShowDetails(0), m_Model(), m_MaterialMap(), m_UpdateModelMatrix(true)
	{
		GX_PROFILE_FUNCTION()

		m_RawData = CreateRef<RawMeshData>(Vertices, Indices);

		if (Mat != nullptr)
		{
			m_Materials.emplace_back(Mat);
		}

		BuildMesh();
	}

	Mesh3D::Mesh3D(const Mesh3D& Mesh)
		: Position(Mesh.Position), Rotation(Mesh.Rotation), Scale(Mesh.Scale), bShowDetails(0), m_Materials(Mesh.m_Materials), m_OverrideMaterial(Mesh.m_OverrideMaterial), m_Model(Mesh.m_Model), m_MaterialMap(Mesh.m_MaterialMap), m_Bounds(Mesh.m_Bounds), m_UpdateModelMatrix(true)
	{
		GX_PROFILE_FUNCTION()

		m_RawData = Mesh.m_RawData;

		BuildMesh();
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
		GX_ENGINE_ASSERT(m_Initialised == true, "Render Data not initialised for the mesh");
		m_RenderData->VAO->Bind();
	}

	void Mesh3D::Disable() const
	{
		GX_ENGINE_ASSERT(m_Initialised == true, "Render Data not initialised for the mesh");
		m_RenderData->VAO->UnBind();
	}

	void Mesh3D::BuildMesh()
	{
		// Create Render Data
		m_RenderData = CreateScope<RenderDataMesh3D>();
		
		uint32_t NumSections = (uint32_t)m_RawData->SectionInfos.size();
		for (uint32_t SectionIndex = 0; SectionIndex < NumSections; SectionIndex++)
		{
			RawMeshData::MeshSectionInfo& SectionInfo = m_RawData->SectionInfos[SectionIndex];
			m_MaterialMap.Add(SectionInfo.MaterialIndex, SectionIndex);


			uint32_t NumIndices = 0;
			if (SectionIndex < NumSections - 1)
				NumIndices = m_RawData->SectionInfos[(size_t)SectionIndex + 1].FirstIndex - SectionInfo.FirstIndex;
			else
				NumIndices = (uint32_t)m_RawData->Indices.size() - SectionInfo.FirstVertexIndex;

			MeshSection Section;
			Section.MaterialIndex = SectionInfo.MaterialIndex;
			Section.FirstVertexIndex = SectionInfo.FirstVertexIndex;
			Section.FirstIndex = SectionInfo.FirstIndex;
			Section.NumTriangles = NumIndices / 3;

			m_RenderData->Sections.emplace_back(Section);
		}

		CalculateBounds();
	}

	void Mesh3D::CalculateBounds()
	{
		m_BoundingBox.reset();
		
		if (m_Bounds.Valid)
		{
			Vector3 Min(m_Bounds.Origin - m_Bounds.Extent);
			Vector3 Max(m_Bounds.Origin + m_Bounds.Extent);
			m_BoundingBox = CreateRef<GM::BoundingBox>(Min, Max);
		}
		else
		{
			m_BoundingBox = CreateRef<GM::BoundingBox>();
			GM::BoundingBox& Box = *m_BoundingBox;
			for (unsigned int i = 0; i < m_RawData->Vertices.size(); i++)
			{
				Box += m_RawData->Vertices[i].Position;
			}

			m_Bounds = GM::BoxBounds(m_BoundingBox.operator*());
		}
	}

	uint32_t Mesh3D::AddMaterial(const Ref<Material>& InMat)
	{
		uint32_t ReturnVal = m_Materials.size();
		m_Materials.emplace_back(InMat);
		return ReturnVal;
	}

	bool Mesh3D::InitResources()
	{
		if (m_Initialised)
			return true;

		if (m_RenderData == nullptr)
			return false;
		
		m_RenderData->VAO = CreateScope<VertexArray>();
		m_RenderData->VBO = CreateRef<VertexBuffer>(&m_RawData->Vertices[0], m_RawData->Vertices.size() * sizeof(Vertex3D));
		const VertexBufferLayout& layout = Vertex3D::VertexLayout();
		m_RenderData->VAO->AddVertexBuffer(m_RenderData->VBO.operator*(), layout);

		m_RenderData->IBO = CreateRef<IndexBuffer>(&m_RawData->Indices[0], m_RawData->Indices.size());
		m_RenderData->VAO->AddIndexBuffer(m_RenderData->IBO.operator*());

		m_Initialised = true;
		return true;
	}

	bool Mesh3D::ReleaseResources()
	{
		m_RenderData->IBO.reset();
		m_RenderData->VBO.reset();
		m_RenderData->VAO.reset();
		
		m_Initialised = false;
		return true;
	}

	Mesh3D::~Mesh3D()
	{
	}
}