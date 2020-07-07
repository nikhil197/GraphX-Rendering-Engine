#include "pch.h"
#include "Importer.h"

#include "Engine/Core/Vertex.h"
#include "Model/Mesh/Mesh3D.h"
#include "Textures/Texture2D.h"
#include "Utilities/EngineUtil.h"
#include "Engine/Core/Materials/Material.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace GraphX
{
	using namespace GM;

	std::shared_ptr<Importer> Importer::s_Importer = nullptr;

	const std::shared_ptr<Importer>& Importer::Get()
	{
		if (s_Importer == nullptr)
		{
			s_Importer.reset(new Importer());
		}

		return s_Importer;
	}

	bool Importer::ImportMesh(const std::string& FilePath, Mesh3D* InMesh, const Ref<Material>& Mat)
	{
		Assimp::Importer AssimpImporter;
		const aiScene* Scene = AssimpImporter.ReadFile(FilePath, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (Scene == nullptr || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || Scene->mRootNode == nullptr)
		{
			return false;
		}

		// Extract meshes from the Scene object
		return BuildMeshFromAssimpScene(FilePath, Scene, InMesh, Mat);
	}

	bool Importer::BuildMeshFromAssimpScene(const std::string& FilePath, const aiScene* Scene, Mesh3D* InMesh, const Ref<Material>& InMat)
	{
		if (!Scene->HasMeshes())
			return false;

		RawMeshData* RawData = InMesh->GetRawData();
		const MaterialToSectionMap& MaterialMap = InMesh->GetMaterialMap();
		std::vector<Vertex3D>& Vertices = RawData->Vertices;
		std::vector<uint32_t>& Indices = RawData->Indices;
		std::vector<RawMeshData::MeshSectionInfo>& Infos = RawData->SectionInfos;
		
		std::unordered_map<unsigned int, uint32_t> AssimpMaterialIndexMap;

		uint32_t VerticesOffset = 0;
		uint32_t IndicesOffset = 0;

		uint32_t VerticesCount = 0;
		uint32_t IndicesCount = 0;
		uint32_t SectionCount = Scene->mNumMeshes;

		// Get the count of vertices and indices in this mesh
		for (unsigned int i = 0; i < Scene->mNumMeshes; i++)
		{
			aiMesh* Mesh = Scene->mMeshes[i];
			if(Mesh->HasPositions())
			{
				VerticesCount += Mesh->mNumVertices;
			}

			if (Mesh->HasFaces())
			{
				for (unsigned int i = 0; i < Mesh->mNumFaces; i++)
				{
					aiFace& Face = Mesh->mFaces[i];
					IndicesCount += Face.mNumIndices;
				}
			}

		}

		// Expand the vertices and indices containers
		RawData->Vertices.resize(VerticesCount);
		RawData->Indices.resize(IndicesCount);
		RawData->SectionInfos.reserve(SectionCount);

		// Extract data from the mesh
		for (unsigned int MeshIndex = 0; MeshIndex < Scene->mNumMeshes; MeshIndex++)
		{
			aiMesh* iMesh = Scene->mMeshes[MeshIndex];

			size_t NumVertices = (size_t)iMesh->mNumVertices;
			// Extract the Vertex positions
			if (iMesh->HasPositions())
			{
				for (size_t i = 0; i < NumVertices; i++)
				{
					Vertices[VerticesOffset + i].Position = Vector3(iMesh->mVertices[i].x, iMesh->mVertices[i].y, iMesh->mVertices[i].z);
				}
			}

			// Extract the Normals
			if (iMesh->HasNormals())
			{
				for (size_t i = 0; i < NumVertices; i++)
				{
					Vertices[VerticesOffset + i].Normal = Vector3(iMesh->mNormals[i].x, iMesh->mNormals[i].y, iMesh->mNormals[i].z);
				}
			}

			// Extract the Texture coordinates
			// TODO: Extract texture coordinates for other channels too
			if (iMesh->HasTextureCoords(0))
			{
				for (size_t i = 0; i < NumVertices; i++)
				{
					Vertices[VerticesOffset + i].TexCoord = Vector2(iMesh->mTextureCoords[0][i].x, iMesh->mTextureCoords[0][i].y);
				}
			}

			// Extract indices
			uint32_t MeshIndicesCount = 0;
			if (iMesh->HasFaces())
			{
				unsigned int NumFaces = iMesh->mNumFaces;
				for (unsigned int i = 0; i < NumFaces; i++)
				{
					const aiFace& iFace = iMesh->mFaces[i];
					for (unsigned int j = 0; j < iFace.mNumIndices; j++)
					{
						Indices[IndicesOffset + MeshIndicesCount + j] = iFace.mIndices[j];
					}

					MeshIndicesCount += iFace.mNumIndices;
				}
			}

			// TODO: Extract vertex colors

			// Textures
			if (Scene->HasMaterials())
			{
				Ref<Material> NewMaterial = nullptr;
				uint32_t MaterialIndex = -1;
				auto itr = AssimpMaterialIndexMap.find(iMesh->mMaterialIndex);
				if (itr != AssimpMaterialIndexMap.end())
				{
					MaterialIndex = itr->second;
					NewMaterial = InMesh->GetMaterial(itr->second);
				}
				else
				{
					NewMaterial = CreateRef<Material>(InMat.operator*());
					aiMaterial* mat = Scene->mMaterials[iMesh->mMaterialIndex];
					unsigned int TexCount = mat->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE);
					aiString* Path = new aiString();
					std::string FileLocation = EngineUtil::ExtractFileLocation(FilePath);
					for (unsigned int i = 0; i < TexCount; i++)
					{
						mat->GetTexture(aiTextureType::aiTextureType_DIFFUSE, i, Path);
						std::string FileName = EngineUtil::ExtractFileName(Path->C_Str());
						NewMaterial->AddTexture(CreateRef<Texture2D>(FileLocation + '\\' + FileName));
					}

					// If no texture is loaded with the model, load the provided materials textures
					// TODO: In case the provided material doesn't have textures either, provide the default material
					if (TexCount == 0)
					{
						NewMaterial->AddTexture(InMat->GetTextures());
					}

					MaterialIndex = InMesh->AddMaterial(NewMaterial);
				}

				RawMeshData::MeshSectionInfo Info;
				Info.FirstVertexIndex = VerticesOffset;
				Info.FirstIndex = IndicesOffset;
				Info.MaterialIndex = MaterialIndex;
				Infos.emplace_back(Info);
			}

			VerticesOffset += iMesh->mNumVertices;
			IndicesOffset += MeshIndicesCount;
		}

		// No Materials were loaded with the mesh, add the provided material (Special case)
		if (Infos.size() == 0)
		{ 
			RawMeshData::MeshSectionInfo Info;
			Info.FirstVertexIndex = 0;
			Info.FirstIndex = 0;
			Info.MaterialIndex = InMesh->AddMaterial(InMat);
			Infos.emplace_back(Info);
		}

		return true;
	}

}