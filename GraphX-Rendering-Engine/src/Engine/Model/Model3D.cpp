#include "pch.h"

#include "Model3D.h"
#include "Mesh/Mesh3D.h"
#include "Materials/Material.h"
#include "Utilities/Importer.h"

namespace GraphX
{
	Model3D::Model3D(const std::string& FilePath, Material* Mat)
		: m_Meshes(new std::vector<Mesh3D*>()), m_FilePath(FilePath)
	{
		Timer timer("Loading Model \"" + m_FilePath + "\"");

		std::vector<std::vector<const Texture*>> Textures;

		bool Loaded = Importer::Get()->ImportModel(m_FilePath, *m_Meshes, Textures);

		if (Loaded)
		{
			// Set the shader for each mesh
			std::vector<Mesh3D*>::iterator itr = m_Meshes->begin();
			std::vector<Mesh3D*>::iterator end = m_Meshes->end();

			unsigned int i = 0;

			while (itr != end)
			{
				Material* NewMat = new Material(*Mat);
				NewMat->AddTexture(Textures[i]);

				(*itr)->SetMaterial(Mat);
				itr++;
				i++;
			}
		}
		else
		{
			GX_ENGINE_ERROR("Failed to load Model {0}", m_FilePath);
		}
	}
}