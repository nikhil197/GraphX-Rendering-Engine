#pragma once

#include "Mesh/Mesh3D.h"

namespace engine
{
	class Model3D
	{
	private:
		/* Meshes making up the model */
		std::vector<class Mesh3D> m_Meshes;

	public:
		/* Constructor */
		Model3D(const std::string& FilePath);

		/* Returns the meshes of the model */
		inline const std::vector<class Mesh3D>& GetMeshes() const { return m_Meshes; }

	private:
		void LoadModel(const std::string& FilePath);
	};
}