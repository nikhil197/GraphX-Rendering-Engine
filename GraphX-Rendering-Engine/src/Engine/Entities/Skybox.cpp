#include "pch.h"

#include "Skybox.h"

#include "Model/Cube.h"

#include "Entities/Camera.h"
#include "Engine/Controllers/CameraController.h"

#include "Textures/CubeMap.h"
#include "Shaders/Shader.h"

#include "Engine/Core/Renderer/Renderer.h"
#include "VertexArray.h"
#include "Buffers/IndexBuffer.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexBufferLayout.h"

namespace GraphX
{
	Skybox::Skybox(const std::string& ShaderFilePath, const std::string& FilePath, const std::vector<std::string>& FileNames, const GM::Matrix4& ViewMat, const GM::Vector4& color, float factor, unsigned int slot, float Speed)
		: m_VAO(CreateScope<VertexArray>()), m_VBO(nullptr), m_IBO(nullptr), m_CubeMap(new CubeMap(FilePath, FileNames)), m_BindingSlot(slot), m_Rotation(0.0f), m_View(ViewMat), m_BlendColor(color), RotationSpeed(Speed), BlendFactor(factor)
	{
		GX_PROFILE_FUNCTION()

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

		std::vector<GM::Vector3> vertices = Cube::GetVertexPositions();

		m_VBO = CreateScope<VertexBuffer>(&vertices[0], vertices.size() * sizeof(GM::Vector3));
		m_IBO = CreateRef<IndexBuffer>(&indices[0], indices.size());

		ShaderLibrary& ShaderLib = Renderer::GetShaderLibrary();
		if (ShaderLib.Exists("Skybox"))
			m_Shader = ShaderLib.GetShader("Skybox");
		else
			m_Shader = ShaderLib.Load(ShaderFilePath, "Skybox");

		VertexBufferLayout layout = {
			{ BufferDataType::Float3 }
		};

		m_VAO->AddVertexBuffer(*m_VBO, layout);
		m_VAO->AddIndexBuffer(*m_IBO);

		m_View(0, 3) = 0.0f;
		m_View(1, 3) = 0.0f;
		m_View(2, 3) = 0.0f;
	}

	void Skybox::Update(float DeltaTime)
	{
		m_Rotation += RotationSpeed * DeltaTime;
		GM::Utility::ClampAngle(m_Rotation);
		m_View = m_View * GM::Rotation(RotationSpeed * DeltaTime, GM::Vector3::YAxis);
	}

	void Skybox::Enable(class Shader& shader, const std::string& Name) const
	{
	}

	void Skybox::Enable() const
	{
		GX_PROFILE_FUNCTION()

		m_VAO->Bind();
		
		m_CubeMap->Bind(m_BindingSlot);
	}

	void Skybox::Disable() const
	{
		GX_PROFILE_FUNCTION()

		m_VAO->UnBind();
		
		m_CubeMap->UnBind();
	}

	Skybox::~Skybox()
	{
		
	}
}