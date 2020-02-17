#include "pch.h"

#include "Model3D.h"
#include "Mesh/Mesh3D.h"
#include "Materials/Material.h"
#include "Utilities/Importer.h"

namespace GraphX
{
	Model3D::Model3D(const std::string& FilePath, const Ref<Material>& Mat)
		: m_Meshes(new std::vector<Ref<Mesh3D>>()), m_FilePath(FilePath)
	{
		GX_ENGINE_INFO("Loading Model {0}", FilePath);
		GX_PROFILE_FUNCTION()

		std::vector<std::vector<Ref<const Texture2D>>> Textures;

		bool Loaded = false;
		{
			GX_PROFILE_SCOPE("Assimp-Load-Model")

			Loaded = Importer::Get()->ImportModel(m_FilePath, *m_Meshes, Textures);
		}

		if (Loaded)
		{
			// Set the shader for each mesh
			std::vector<Ref<Mesh3D>>::iterator itr = m_Meshes->begin();
			std::vector<Ref<Mesh3D>>::iterator end = m_Meshes->end();

			unsigned int i = 0;

			while (itr != end)
			{
				Ref<Material> NewMat = Mat;
				NewMat->AddTexture(Textures[i]);

				(*itr)->SetMaterial(NewMat);
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