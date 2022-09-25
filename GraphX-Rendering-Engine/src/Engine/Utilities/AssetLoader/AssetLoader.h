#pragma once

#include <future>

namespace GraphX
{
	class Mesh3D;
	class Texture2D;
	class Material;
	class AssetLoader
	{
	public:
		/* Loads and returns a Mesh 3D synchronously */
		static Ref<Mesh3D> LoadMesh3D(const std::string& FilePath, const Ref<Material>& Mat);

		/* Loads the Mesh3D asynchronously and calls the given CallBack when loaded */
		static void LoadMesh3D(const std::string& FilePath, const Ref<Material>& Mat, const std::function<void(const Ref<Mesh3D>&)>& CallBackFn);

		/* Loads the Texture 2D asynchronously and calls the given Callback function when loaded */
		static void LoadTexture2D(const std::string& FilePath, const std::function<void(const Ref<Texture2D>&)>& CallBackFn);

	private:
		static void LoadMesh3D_Internal(std::string FilePath, const Ref<Material>& Mat, const std::function<void(const Ref<Mesh3D>&)>& CallBackFn);

		static void LoadTexture2D_Internal(std::string FilePath, const std::function<void(const Ref<Texture2D>&)>& CallBackFn);

		// vector to store the future objects returned by the async method
		static std::vector<std::future<void>> s_Futures;

		/* A map to store the loaded models */
		static std::unordered_map<std::string, class Mesh3D*> s_LoadedModels;
	};
}