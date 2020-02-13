#pragma once

struct aiScene;

namespace GraphX
{
	class Mesh3D;
	class Material;

	class Model3D
	{
	public:
		/* Constructor */
		Model3D(const std::string& FilePath, const Ref<Material>& Mat);

		/* Returns the meshes of the model */
		inline const Ref<std::vector<Ref<Mesh3D>>>& GetMeshes() const { return m_Meshes; }

	private:
		/* Meshes making up the model */
		Ref<std::vector<Ref<Mesh3D>>> m_Meshes;

		/* Path of the file */
		std::string m_FilePath;
	};
}