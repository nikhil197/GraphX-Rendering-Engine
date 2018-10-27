#pragma once

#include <string>
#include <unordered_map>

namespace engine
{
	namespace shaders
	{
		// Struct to hold the both the shaders' source
		struct ShaderSource
		{
			std::string VertexShaderSource;
			std::string FragmentShaderSource;
		};

		class Shader
		{
		private: 
			/* Path to the shader source file */
			std::string m_FilePath;

			/* ID of the shader */
			unsigned int m_RendererID;

			/* To cache the uniform locations */
			std::unordered_map<std::string, int> m_UniformLocations;

		public:
			/* filePath is the path to the source file */
			Shader(const std::string& filePath);

			~Shader();

			/* Bind the shader */
			void Bind() const;

			/* Un Bind the shader */
			void UnBind() const;

			void SetUniform4f(float r, float g, float b, float a, const char* Name);

		private:
			/* Parse the source file and get the shader source codes */
			ShaderSource ParseShaderSource(const std::string& filePath);

			/* Compile the shader source extracted from the file */
			unsigned int CompileShader(unsigned int type, const std::string& shaderSouce);

			/* Create a program and attach the shaders to it */
			unsigned int CreateShader(const std::string& vertexSource, const std::string& fragmentSource);

			/* Returns the location of the uniform with the given name */
			int GetLocation(const std::string& Name);
		};
	}
}