#pragma once

struct aiScene;

namespace engine
{
	class Model3D
	{
	private:
		/* Meshes making up the model */
		std::vector<class Mesh3D*>* m_Meshes;

		/* Shader used for rendering the model */
		class Shader& m_Shader;

	public:
		/* Constructor */
		Model3D(const std::string& FilePath, class Shader& shader);

		/* Returns the meshes of the model */
		inline const std::vector<class Mesh3D*>& GetMeshes() const { return *m_Meshes; }

	private:
		/* Load the model from the explorer*/
		void LoadModel(const std::string& FilePath);

		/* Process the scene retreived from the file */
		void ProcessScene(const aiScene* Scene);
	};
}