#pragma once

struct aiScene;

namespace GraphX
{
	/**
	 * Importer used to import different types of objects (Currently uses Assimp)
	 */
	class Importer
	{
	public:
		/* Returns the only Importer Instance */
		static const std::shared_ptr<Importer>& Get();

		/* Imports the scene from the specified file as a Mesh3D into the provided mesh. Returns if the model was loaded correctly or not */
		bool ImportMesh(const std::string& FilePath, class Mesh3D* InMesh, const Ref<class Material>& Mat);

	private:
		Importer() {}

		/* Processes the scene loaded using the Assimp library */
		bool BuildMeshFromAssimpScene(const std::string& FilePath, const aiScene* Scene, class Mesh3D* InMesh, const Ref<class Material>& Mat);

	private:
		/* Importer Instance */
		static std::shared_ptr<Importer> s_Importer;
	};
}