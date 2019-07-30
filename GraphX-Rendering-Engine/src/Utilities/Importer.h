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
		static Importer* Get();

		/* Imports the Model into provides container. Returns if the model was loaded correctly or not */
		bool ImportModel(const std::string& FilePath, /* out */ std::vector<class Mesh3D*>& Meshes);

	private:
		Importer() {}

		/* Processes the scene loaded using the Assimp library */
		bool ProcessAssimpScene(const std::string& FilePath, const aiScene* Scene, std::vector<Mesh3D*>& Meshes);

	private:
		/* Importer Instance */
		static Importer* s_Importer;
	};
}