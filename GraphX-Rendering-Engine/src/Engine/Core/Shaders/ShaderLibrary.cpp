#include "pch.h"
#include "ShaderLibrary.h"

#include "Shader.h"

namespace GraphX
{
	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		GX_ASSERT(!Exists(name), "Shader {0} already exists in the library", shader->GetName());
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		const std::string& name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& FilePath, const std::string& name)
	{
		Ref<Shader> shader = CreateRef<Shader>(FilePath, name);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& VertexSource, const std::string& FragSrc)
	{
		Ref<Shader> shader = CreateRef<Shader>(name, VertexSource, FragSrc);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::GetShader(const std::string& name) const
	{
		GX_ASSERT(Exists(name), "Shader {0} not found", name);
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}