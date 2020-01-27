#pragma once

struct aiScene;

namespace GraphX
{
	/* NOTE: Meshes of the model need to be deleted explicitly */
	class Model3D
	{
	public:
		/* Constructor */
		Model3D(const std::string& FilePath, class Material* Mat);

		/* Returns the meshes of the model */
		inline const std::vector<class Mesh3D*>& GetMeshes() const { return *m_Meshes; }

	private:
		/* Meshes making up the model */
		std::vector<class Mesh3D*>* m_Meshes;

		/* Path of the file */
		std::string m_FilePath;
	};
}