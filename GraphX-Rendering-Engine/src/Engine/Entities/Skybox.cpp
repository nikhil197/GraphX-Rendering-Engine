#include "pch.h"

#include "Skybox.h"

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
	Skybox::Skybox(const std::string& FilePath, const std::vector<std::string>& FileNames, const GM::Vector4& color, float factor, float Speed)
		: RotationSpeed(Speed), BlendFactor(factor), m_CubeMap(new CubeMap(FilePath, FileNames)), m_Rotation(), m_Model(GM::RotationTranslationMatrix(m_Rotation, GM::Vector3::ZeroVector)), m_Tint(color)
	{
	}

	void Skybox::Update(float DeltaTime)
	{
		GX_PROFILE_FUNCTION()

		if (RotationSpeed != 0.0f)
		{
			m_Rotation.Yaw += RotationSpeed * DeltaTime;
			GM::Utility::ClampAngle(m_Rotation.Yaw);
			GM::RotationTranslationMatrix::Make(m_Model, m_Rotation, GM::Vector3::ZeroVector);
		}
	}

	void Skybox::Enable(class Shader& shader, const std::string& Name) const
	{
	}

	void Skybox::Enable() const
	{
		GX_PROFILE_FUNCTION()

		m_CubeMap->Bind(EngineConstants::SkyboxBindingSlot);
	}

	void Skybox::Disable() const
	{
		GX_PROFILE_FUNCTION()

		m_CubeMap->UnBind();
	}

	Skybox::~Skybox()
	{ }
}