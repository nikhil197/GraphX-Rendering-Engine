#pragma once

namespace GraphX
{
	class Shader;

	class ShaderLibrary
	{
	public:
		/* Adds a new shader to the library */
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader);

		// Creates a new shader and returns a reference to it
		Ref<Shader> Load(const std::string& FilePath, const std::string& Name = "");
		Ref<Shader> Load(const std::string& name, const std::string& VertexSource, const std::string& FragSrc);

		// Returns a shader with the given name
		Ref<Shader> GetShader(const std::string& name);

		// Returns whether the shader with the give name exists or not
		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}