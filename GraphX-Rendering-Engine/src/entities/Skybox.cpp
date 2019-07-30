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
	Skybox::Skybox(const std::string& ShaderFilePath, const std::string& FilePath, const std::vector<std::string>& FileNames, const Camera& Camera, const gm::Vector4& color, float factor, unsigned int slot, float Speed)
		: m_VAO(new VertexArray()), m_VBO(nullptr), m_IBO(nullptr), m_Shader(new Shader(ShaderFilePath)), m_CubeMap(new CubeMap(FilePath, FileNames)), m_Camera(Camera), m_BindingSlot(slot), m_Rotation(0.0f), m_BlendColor(color), RotationSpeed(Speed), BlendFactor(factor)
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
		m_VAO->AddIndexBuffer(*m_IBO);

		m_View = m_Camera.GetViewMatrix();
		m_View[0][3] = 0.0f;
		m_View[1][3] = 0.0f;
		m_View[2][3] = 0.0f;

		m_Shader->Bind();
		m_Shader->SetUniform4f("u_BlendColor", m_BlendColor);
	}

	void Skybox::Update(float DeltaTime)
	{
		m_Rotation += RotationSpeed * DeltaTime;
		gm::MathUtil::ClampAngle(m_Rotation);
		m_View = m_View * gm::Rotation(RotationSpeed * DeltaTime, gm::Vector3::YAxis);
	}

	void Skybox::Enable(class Shader& shader, const std::string& Name) const
	{
	}

	void Skybox::Enable() const
	{
		GLCall(glDepthMask(GL_FALSE));
		GLCall(glDisable(GL_CULL_FACE));

		m_VAO->Bind();
		
		m_Shader->Bind();
		m_CubeMap->Bind(m_BindingSlot);

		// Set the uniforms
		m_Shader->SetUniformMat4f("u_View", m_View);
		m_Shader->SetUniformMat4f("u_Projection", m_Camera.GetProjectionMatrix());
		m_Shader->SetUniform1i("u_Skybox", m_BindingSlot);
		m_Shader->SetUniform1f("u_BlendFactor", BlendFactor);
	}

	void Skybox::Disable() const
	{
		GLCall(glDepthMask(GL_TRUE));
		GLCall(glEnable(GL_CULL_FACE));

		m_VAO->UnBind();
		
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