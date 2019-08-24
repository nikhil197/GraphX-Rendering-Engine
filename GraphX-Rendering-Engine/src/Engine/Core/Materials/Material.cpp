#include "pch.h"

#include "Material.h"
#include "Shaders/Shader.h"
#include "Textures/Texture.h"

namespace GraphX
{
	Material::Material(Shader* shader)
		: m_BaseColor(Vector4()), m_Shader(shader)
	{
	}

	Material::Material(const Material& Other)
		: m_BaseColor(Other.m_BaseColor), m_Specular(Other.m_Specular), m_Shininess(Other.m_Shininess), m_Shader(Other.m_Shader)
	{
	}

	void Material::Bind()
	{
		m_Shader->Bind();
		m_Shader->SetUniform1f("u_Reflectivity", m_Specular);
		m_Shader->SetUniform1f("u_Shininess", m_Shininess);
		m_Shader->SetUniform4f("u_TintColor", m_BaseColor);

		// Bind the textures
		int NumTex = m_Textures.size();
		for (int i = 0; i < NumTex; i++)
		{
			const Texture* texture = m_Textures[i];
			texture->Bind(i);
			m_Shader->SetUniform1i((std::string("u_Texture") + std::to_string(i)).c_str(), i);
		}
	}

	void Material::AddTexture(const Texture* Tex)
	{
		m_Textures.emplace_back(Tex);
	}

	void Material::AddTexture(const std::vector<const Texture*>& Textures)
	{
		for (const Texture* Tex : Textures)
		{
			m_Textures.emplace_back(Tex);
		}
	}
}