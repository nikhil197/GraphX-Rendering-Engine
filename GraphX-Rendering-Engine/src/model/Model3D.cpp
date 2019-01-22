#include "pch.h"

#include "Model3D.h"
#include "Mesh/Mesh3D.h"
#include "Renderer/Renderer3D.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace engine
{
	Model3D::Model3D(const std::string& FilePath)
	{
		LoadModel(FilePath);
	}

	void Model3D::LoadModel(const std::string& FilePath)
	{
		Assimp::Importer Importer;
		const aiScene* Scene = Importer.ReadFile(FilePath, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (Scene == nullptr || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || Scene->mRootNode == nullptr)
		{
			GX_ENGINE_ERROR("Failed to load Model {0}", FilePath);
			return;
		}
	}
}