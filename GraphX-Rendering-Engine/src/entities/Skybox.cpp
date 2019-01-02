#include "pch.h"

#include "Skybox.h"
#include "Entities/Camera.h"
#include "Model/Cube.h"

#include "Textures/CubeMap.h"
#include "Shaders/Shader.h"

#include "VertexArray.h"
#include "Buffers/IndexBuffer.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexBufferLayout.h"

namespace engine
{
	Skybox::Skybox(const std::string& ShaderFilePath, const std::string& FilePath, const std::vector<std::string>& FileNames, const Camera& Camera)
		: m_VAO(new VertexArray()), m_VBO(nullptr), m_IBO(nullptr), m_Shader(new Shader(ShaderFilePath)), m_CubeMap(new CubeMap(FilePath, FileNames)), m_Camera(Camera)
	{
		std::vector<unsigned int> indices = Cube::GetIndices();
		// Top face
		indices[6]  = 7;
		indices[7]  = 3;
		indices[8]  = 6;
		indices[9]  = 6;
		indices[10] = 3;
		indices[11] = 2;
		// Bottom face
		indices[18] = 0;
		indices[19] = 4;
		indices[20] = 1;
		indices[21] = 1;
		indices[22] = 4;
		indices[23] = 5;

		std::vector<gm::Vector3> vertices = Cube::GetVertexPositions();

		m_VBO = new VertexBuffer(&vertices[0], vertices.size() * sizeof(gm::Vector3));
		m_IBO = new IndexBuffer(&indices[0], indices.size());

		VertexBufferLayout layout;
		layout.Push<float>(gm::Vector3::Components);
		
		m_VAO->AddBuffer(*m_VBO, layout);
	}

	void Skybox::Enable() const
	{
		gm::Matrix4 view = m_Camera.GetViewMatrix();
		view[0][3] = 0.0f;
		view[1][3] = 0.0f;
		view[2][3] = 0.0f;
		view[3][3] = 0.0f;

		m_VAO->Bind();
		m_IBO->Bind();
		m_Shader->Bind();
		m_CubeMap->Bind();

		// Set the uniforms
		m_Shader->SetUniformMat4f("u_View", view);
		m_Shader->SetUniformMat4f("u_Projection", m_Camera.GetPerspectiveProjectionMatrix());
		m_Shader->SetUniform1i("u_Skybox", 0);
	}

	void Skybox::Disable() const
	{
		m_VAO->UnBind();
		m_IBO->UnBind();
		m_Shader->UnBind();
		m_CubeMap->UnBind();
	}

	Skybox::~Skybox()
	{
		delete m_VAO;
		delete m_VBO;
		delete m_IBO;
		delete m_CubeMap;
	}
}