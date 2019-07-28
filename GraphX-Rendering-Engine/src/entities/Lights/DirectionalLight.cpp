#include "pch.h"
#include "DirectionalLight.h"

#include "Shaders/Shader.h"
#include "Entities/Camera.h"

namespace engine
{
	DirectionalLight::DirectionalLight(const gm::Vector4& Color, const gm::Vector3& LightDir, float Intensity)
		: Light(Color, gm::Projection::Ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f), Intensity), Direction(LightDir)
	{
		m_LightShadowInfo->LightViewProjMat = m_LightShadowInfo->LightProjMat * gm::View::LookAt(gm::Vector3(-2.0f, 4.0f, -1.0f), gm::Vector3(0.0f), gm::Vector3::YAxis);
	}

	void DirectionalLight::Update(float DeltaTime)
	{
		Light::Update(DeltaTime);
	}

	void DirectionalLight::Enable(Shader& shader, const std::string& LightName) const
	{
		Light::Enable(shader, LightName);

		shader.SetUniform3f((LightName + ".Direction").c_str(), Direction);
	}

	void DirectionalLight::Disable() const
	{
		Light::Disable();
	}
}