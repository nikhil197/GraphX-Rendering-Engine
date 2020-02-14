#pragma once

struct aiScene;

namespace GraphX
{
	/**
	 * Importer used to import different types of objects (Currently supports Assimp)
	 */
	class Importer
	{
	public:
		/* Returns the only Importer Instance */
		static const std::shared_ptr<Importer>& Get();

		/* Imports the Model into provides container. Returns if the model was loaded correctly or not */
		bool ImportModel(const std::string& FilePath, /* out */ std::vector<Ref<class Mesh3D>>& Meshes, /* out */ std::vector<std::vector<Ref<const class Texture2D>>>& Textures);

	private:
		Importer() {}

		/* Processes the scene loaded using the Assimp library */
		bool ProcessAssimpScene(const std::string& FilePath, const aiScene* Scene, std::vector<Ref<Mesh3D>>& Meshes, std::vector<std::vector<Ref<const Texture2D>>>& Textures);

	private:
		/* Importer Instance */
		static std::shared_ptr<Importer> s_Importer;
	};
}