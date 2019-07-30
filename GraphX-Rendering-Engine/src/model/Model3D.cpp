#include "pch.h"

#include "Model3D.h"
#include "Mesh/Mesh3D.h"
#include "Shaders/Shader.h"
#include "Utilities/Importer.h"

namespace GraphX
{
	Model3D::Model3D(const std::string& FilePath, Shader* shader)
		: m_Meshes(new std::vector<Mesh3D*>()), m_Shader(shader), m_FilePath(FilePath)
	{
		Timer timer("Loading Model \"" + m_FilePath + "\"");

		bool Loaded = Importer::Get()->ImportModel(m_FilePath, *m_Meshes);

		if (Loaded)
		{
			// Set the shader for each mesh
			std::vector<Mesh3D*>::iterator itr = m_Meshes->begin();
			std::vector<Mesh3D*>::iterator end = m_Meshes->end();

			while (itr != end)
			{
				(*itr)->SetShader(m_Shader);
				itr++;
			}
		}
		else
		{
			GX_ENGINE_ERROR("Failed to load Model {0}", m_FilePath);
		}
	}
}