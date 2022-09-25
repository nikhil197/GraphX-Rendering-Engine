#include "pch.h"
#include "AssetLoader.h"

#include "Engine/Model/Mesh/Mesh3D.h"

#include "Engine/Core/Textures/Texture2D.h"

#include "Engine/Subsystems/Multithreading/Multithreading.h"

namespace GraphX
{
	std::vector<std::future<void>> AssetLoader::s_Futures;

	std::unordered_map<std::string, Mesh3D*> AssetLoader::s_LoadedModels;

	Ref<Mesh3D> AssetLoader::LoadMesh3D(const std::string& FilePath, const Ref<Material>& Mat)
	{
		return Mesh3D::Load(FilePath, Mat);
	}

	void AssetLoader::LoadMesh3D(const std::string& FilePath, const Ref<Material>& Mat, const std::function<void(const Ref<Mesh3D>&)>& CallBackFn)
	{
		GX_ENGINE_ASSERT(CallBackFn != nullptr, "Callback function for Loading Mesh 3D async not provided");
		s_Futures.emplace_back(Async<void>(AsyncExecutionPolicy::ThreadPool, std::bind(&AssetLoader::LoadMesh3D_Internal, FilePath, Mat, CallBackFn)));
	}

	void AssetLoader::LoadMesh3D_Internal(std::string FilePath, const Ref<Material>& Mat, const std::function<void(const Ref<Mesh3D>&)>& CallBackFn)
	{
		// Load the mesh
		Ref<Mesh3D> Mesh = Mesh3D::Load(FilePath, Mat);

		// Call the Callback function with the loaded mesh
		CallBackFn(Mesh);
	}

	void AssetLoader::LoadTexture2D(const std::string& FilePath, const std::function<void(const Ref<Texture2D>&)>& CallBackFn)
	{
		GX_ENGINE_ASSERT(CallBackFn != nullptr, "Callback function for Loading Texture2D async not provided");
		s_Futures.emplace_back(Async<void>(AsyncExecutionPolicy::ThreadPool, std::bind(&AssetLoader::LoadTexture2D_Internal, FilePath, CallBackFn)));
	}

	void AssetLoader::LoadTexture2D_Internal(std::string FilePath, const std::function<void(const Ref<Texture2D>&)>& CallBackFn)
	{
		// Load the Texture
		Ref<Texture2D> Texture = CreateRef<Texture2D>(FilePath);
		
		// Call the Callback function with the loaded mesh
		CallBackFn(Texture);
	}
}