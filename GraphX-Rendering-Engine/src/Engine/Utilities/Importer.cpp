#include "pch.h"
#include "Importer.h"

#include "Model/Mesh/Vertex.h"
#include "Model/Mesh/Mesh3D.h"
#include "Textures/Texture.h"
#include "Utilities/EngineUtil.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace GraphX
{
	using namespace GraphXMaths;

	Importer* Importer::s_Importer = nullptr;

	Importer* Importer::Get()
	{
		if (s_Importer == nullptr)
		{
			s_Importer = new Importer();
		}

		return s_Importer;
	}

	bool Importer::ImportModel(const std::string& FilePath, std::vector<class Mesh3D*>& Meshes, std::vector<std::vector<const Texture*>>& Textures)
	{
		Assimp::Importer AssimpImporter;
		const aiScene* Scene = AssimpImporter.ReadFile(FilePath, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (Scene == nullptr || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || Scene->mRootNode == nullptr)
		{
			return false;
		}

		// Extract meshes from the Scene object
		return ProcessAssimpScene(FilePath, Scene, Meshes, Textures);
	}

	bool Importer::ProcessAssimpScene(const std::string& FilePath, const aiScene* Scene, std::vector<Mesh3D*>& Meshes, std::vector<std::vector<const Texture*>>& Textures)
	{
		if (!Scene->HasMeshes())
			return false;

		for (unsigned int i = 0; i < Scene->mNumMeshes; i++)
		{
			aiMesh* Mesh = Scene->mMeshes[i];

			// Extract the Vertex positions
			std::vector<Vector3>* vertexPositions = nullptr;
			if (Mesh->HasPositions())
			{
				vertexPositions = new std::vector<Vector3>();

				for (unsigned int i = 0; i < Mesh->mNumVertices; i++)
				{
					vertexPositions->emplace_back(Vector3(Mesh->mVertices[i].x, Mesh->mVertices[i].y, Mesh->mVertices[i].z));
				}
			}

			// Extract the Normals
			std::vector<Vector3>* normals = nullptr;
			if (Mesh->HasNormals())
			{
				normals = new std::vector<Vector3>();
				for (unsigned int i = 0; i < Mesh->mNumVertices; i++)
				{
					normals->emplace_back(Vector3(Mesh->mNormals[i].x, Mesh->mNormals[i].y, Mesh->mNormals[i].z));
				}
			}

			// Extract the Texture coordinates
			std::vector<Vector2>* texCoords = new std::vector<Vector2>();
			if (Mesh->HasTextureCoords(0))
			{
				for (unsigned int i = 0; i < Mesh->mNumVertices; i++)
				{
					texCoords->emplace_back(Vector2(Mesh->mTextureCoords[0][i].x, Mesh->mTextureCoords[0][i].y));
				}
			}

			// Extract indices
			std::vector<unsigned int>* indices = nullptr;
			if (Mesh->HasFaces())
			{
				indices = new std::vector<unsigned int>();
				for (unsigned int i = 0; i < Mesh->mNumFaces; i++)
				{
					aiFace Face = Mesh->mFaces[i];
					for (unsigned int j = 0; j < Face.mNumIndices; j++)
					{
						indices->emplace_back(Face.mIndices[j]);
					}
				}
			}

			// TODO: Add for vertex colors

			// Textures
			std::vector<const Texture*>* textures = nullptr;
			if (Scene->HasMaterials())
			{
				textures = new std::vector<const Texture*>();
				aiMaterial* mat = Scene->mMaterials[Mesh->mMaterialIndex];
				unsigned int TexCount = mat->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE);
				aiString* Path = new aiString();
				std::string FileLocation = EngineUtil::ExtractFileLocation(FilePath);
				for (unsigned int i = 0; i < TexCount; i++)
				{
					mat->GetTexture(aiTextureType::aiTextureType_DIFFUSE, i, Path);
					std::string FileName = EngineUtil::ExtractFileName(Path->C_Str());
					textures->emplace_back(new Texture(FileLocation + '\\' + FileName));
				}
			}

			// Combine all the attributes into vertex array
			std::vector<Vertex3D>* vertices = nullptr;
			if (vertexPositions != nullptr && normals != nullptr)
			{
				vertices = new std::vector<Vertex3D>();
				for (unsigned int i = 0; i < Mesh->mNumVertices; i++)
				{
					Vertex3D vertex;
					vertex.Position = vertexPositions->at(i);
					vertex.Normal = normals->at(i);
					vertex.TexCoord = (texCoords->size() > i) ? texCoords->at(i) : Vector2::ZeroVector;
					vertices->emplace_back(vertex);
				}
			}

			// Create the mesh
			Mesh3D* mMesh = nullptr;
			if (vertices != nullptr && textures != nullptr)
			{
				mMesh = new Mesh3D(Vector3::ZeroVector, Vector3::ZeroVector, Vector3::UnitVector, *vertices, *indices);
			}

			if (mMesh == nullptr)
			{
				return false;
			}

			// Store the mesh and the textures
			Meshes.emplace_back(mMesh);
			Textures.emplace_back(std::vector<const Texture*>());
			for (unsigned int index = 0; index < textures->size(); index++)
			{
				Textures[i].emplace_back(textures->at(index));
			}

			// Clean up the memory
			delete vertexPositions;
			delete normals;
			delete texCoords;
			delete indices;
			delete vertices;
			delete textures;
		}
		return true;
	}

}