#include "pch.h"

#include "Model3D.h"
#include "Mesh/Vertex.h"
#include "Mesh/Mesh3D.h"
#include "Renderer/Renderer3D.h"
#include "Shaders/Shader.h"
#include "Textures/Texture.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace engine
{
	Model3D::Model3D(const std::string& FilePath, Shader& shader)
		: m_Meshes(new std::vector<Mesh3D*>()), m_Shader(shader)
	{
		LoadModel(FilePath);
	}

	void Model3D::LoadModel(const std::string& FilePath)
	{
		Timer timer("Loading Model \"" + FilePath + "\"");
		Assimp::Importer Importer;
		const aiScene* Scene = Importer.ReadFile(FilePath, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (Scene == nullptr || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || Scene->mRootNode == nullptr)
		{
			GX_ENGINE_ERROR("Failed to load Model {0}", FilePath);
			return;
		}

		// Extract meshes from the Scene object
		ProcessScene(Scene);
	}

	void Model3D::ProcessScene(const aiScene* Scene)
	{
		if (!Scene->HasMeshes())
			return;

		for (unsigned int i = 0; i < Scene->mNumMeshes; i++)
		{
			aiMesh* Mesh = Scene->mMeshes[i];

			// Extract the Vertex positions
			std::vector<gm::Vector3>* vertexPositions = nullptr;
			if (Mesh->HasPositions())
			{
				vertexPositions = new std::vector<gm::Vector3>();

				for (unsigned int i = 0; i < Mesh->mNumVertices; i++)
				{
					gm::Vector3 vertex(Mesh->mVertices[i].x, Mesh->mVertices[i].y, Mesh->mVertices[i].z);
					vertexPositions->emplace_back(vertex);
				}
			}

			// Extract the Normals
			std::vector<gm::Vector3>* normals = nullptr;
			if (Mesh->HasNormals())
			{
				normals = new std::vector<gm::Vector3>();
				for (unsigned int i = 0; i < Mesh->mNumVertices; i++)
				{
					gm::Vector3 normal(Mesh->mNormals[i].x, Mesh->mNormals[i].y, Mesh->mNormals[i].z);
					normals->emplace_back(normal);
				}
			}

			// Extract the Texture coordinates
			std::vector<gm::Vector2>* texCoords = new std::vector<gm::Vector2>();
			if (Mesh->HasTextureCoords(0))
			{
				for (unsigned int i = 0; i < Mesh->mNumVertices; i++)
				{
					gm::Vector2 texCoord(Mesh->mTextureCoords[0][i].x, Mesh->mTextureCoords[0][i].y);
					texCoords->emplace_back(texCoord);
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
						indices->emplace_back(Face.mIndices[j]);
				}
			}

			// TODO: Add for vertex colors

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
					vertex.TexCoord = (texCoords->size() > i) ? texCoords->at(i) : gm::Vector2::ZeroVector;
					vertices->emplace_back(vertex);
				}
			}

			// Create the mesh
			Mesh3D* mMesh = nullptr;
			if (vertices != nullptr)
			{
				mMesh = new Mesh3D(gm::Vector3::ZeroVector, gm::Vector3::ZeroVector, gm::Vector3(0.01f, 0.01f, 0.01f), m_Shader, std::vector<const Texture*>(), *vertices, *indices);
			}
			
			if (mMesh == nullptr)
			{
				GX_ENGINE_ERROR("Unable to import the model");
				return;
			}

			// Store the mesh
			m_Meshes->emplace_back(mMesh);

			// Clean up the memory
			delete vertexPositions;
			delete normals;
			delete texCoords;
			delete indices;
			delete vertices;
		}
	}
}