#pragma once

namespace GraphX
{
	// Struct to hold the both the shaders' source
	struct ShaderSource
	{
		std::string VertexShaderSource;
		std::string FragmentShaderSource;
	};

	class Shader
	{
	public:
		/* filePath is the path to the source file */
		Shader(const std::string& filePath, const std::string& name = "");

		Shader(const std::string& name, const std::string& vertexShaderSrc, const std::string& fragShaderSrc);

		~Shader();

		/* Bind the shader */
		void Bind() const;

		/* Un Bind the shader */
		void UnBind() const;

		/* Set uniforms */
		void SetUniform1i(const char* Name, int val);

		void SetUniform2i(const char* Name, int v1, int v2);

		void SetUniform1f(const char* Name, float val);

		void SetUniform2f(const char* Name, float a, float b);

		void SetUniform2f(const char* Name, const GM::Vector2& Vec);

		void SetUniform3f(const char* Name, float r, float g, float b);

		void SetUniform3f(const char* Name, const GM::Vector3& Vec);

		void SetUniform4f(const char* Name, float r, float g, float b, float a);

		void SetUniform4f(const char* Name, const GM::Vector4& Vec);

		void SetUniform4f(const char* Name, const GM::Vector2& Vec1, const GM::Vector2& Vec2);

		void SetUniformMat3f(const char* Name, const GM::Matrix3& Mat);

		void SetUniformMat4f(const char* Name, const GM::Matrix4& Mat);

		// Returns the name for the shader
		const std::string& GetName() const { return m_Name; }

	private:
		/* Parse the source file and get the shader source codes */
		ShaderSource ParseShaderSource(const std::string& filePath);

		/* Compile the shader source extracted from the file */
		unsigned int CompileShader(unsigned int type, const std::string& shaderSouce);

		/* Create a program and attach the shaders to it */
		unsigned int CreateShader(const std::string& vertexSource, const std::string& fragmentSource);

		/* Returns the location of the uniform with the given name */
		int GetLocation(const char* Name);

	private:
		/* Name to the shader source file */
		std::string m_Name;

		/* ID of the shader */
		unsigned int m_RendererID;

		/* To cache the uniform locations */
		std::unordered_map<std::string, int> m_UniformLocations;
	};
}