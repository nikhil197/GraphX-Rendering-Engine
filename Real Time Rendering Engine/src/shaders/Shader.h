#pragma once

#include <string>

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

		public:
			/* filePath is the path to the source file */
			Shader(const std::string& filePath);

			~Shader();

			/* Bind the shader */
			void Bind() const;

			/* Un Bind the shader */
			void UnBind() const;

		private:
			/* Parse the source file and get the shader source codes */
			ShaderSource ParseShaderSource(const std::string& filePath);

			/* Compile the shader source extracted from the file */
			unsigned int CompileShader(unsigned int type, const std::string& shaderSouce);

			/* Create a program and attach the shaders to it */
			unsigned int CreateShader(const std::string& vertexSource, const std::string& fragmentSource);
		};
	}
}