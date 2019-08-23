#include "pch.h"
#include "DirectionalLight.h"

#include "Shaders/Shader.h"
#include "Entities/Camera.h"

namespace GraphX
{
	DirectionalLight::DirectionalLight(const GraphXMaths::Vector4& Color, const GraphXMaths::Vector3& LightDir, float Intensity)
		: Light(Color, GraphXMaths::Projection::Ortho(-100.0f, 100.0f, -50.0f, 50.0f, -200.0f, 200.0f), Intensity), Direction(LightDir)
	{
	}

	void DirectionalLight::Update(float DeltaTime)
	{
		Light::Update(DeltaTime);
		m_LightShadowInfo->LightViewProjMat = m_LightShadowInfo->LightProjMat * GraphXMaths::View::LookAt(GraphXMaths::Vector3(0.0f), Direction, GraphXMaths::Vector3::YAxis);
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