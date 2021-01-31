#include "pch.h"

#include "Material.h"
#include "Shaders/Shader.h"
#include "Textures/Texture2D.h"

#include "Engine/Utilities/AssetLoader/AssetLoader.h"

namespace GraphX
{
	Material::Material(const Shader& shader)
		: m_BaseColor(Vector4()), m_Shader(CreateRef<Shader>(shader))
	{
	}

	Material::Material(const Ref<Shader>& shader)
		: m_BaseColor(Vector4()), m_Shader(shader)
	{}

	Material::Material(const Material& Other)
		: m_BaseColor(Other.m_BaseColor), m_Specular(Other.m_Specular), m_Shininess(Other.m_Shininess), m_Shader(Other.m_Shader)
	{
	}

	void Material::Bind()
	{
		GX_PROFILE_FUNCTION()

		m_Shader->Bind();
		m_Shader->SetUniform1f("u_Reflectivity", m_Specular);
		m_Shader->SetUniform1f("u_Shininess", m_Shininess);
		m_Shader->SetUniform4f("u_TintColor", m_BaseColor);

		// Bind the textures
		int NumTex = m_Textures.size();

		// TODO: Also handle the case of more than 9 texture units i.e. 2-digit texture number
		static std::string TexName("u_Texture ");
		for (int i = 0; i < NumTex; i++)
		{
			const Ref<const Texture2D>& texture = m_Textures[i];
			texture->Bind(i);
			TexName[TexName.length() - 1] = '0' + i;
			m_Shader->SetUniform1i(TexName.c_str(), i);
		}
	}

	void Material::AddTexture(const Ref<const Texture2D>& Tex)
	{
		std::lock_guard<std::mutex> lock(m_TextureMutex);
		m_Textures.emplace_back(Tex);
	}

	void Material::AddTexture(const std::vector<Ref<const Texture2D>>& Textures)
	{
		std::lock_guard<std::mutex> lock(m_TextureMutex);
		for (const Ref<const Texture2D>& Tex : Textures)
		{
			m_Textures.emplace_back(Tex);
		}
	}

	void Material::LoadTexture(const std::string& FilePath)
	{
		AssetLoader::LoadTexture2D(FilePath, std::bind(static_cast<void(Material::*)(const Ref<const Texture2D>&)>(&Material::AddTexture), this, std::placeholders::_1));
	}
}