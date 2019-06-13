#include "pch.h"

#include "PointLight.h"
#include "Shaders/Shader.h"
#include "Entities/Camera.h"

namespace engine
{
	PointLight::PointLight(const gm::Vector3& Pos, const gm::Vector4& Col, float Intensity, const gm::Vector3& AttenuationFactors)
		: Light(Col, gm::Projection::Ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 1000.0f), Intensity), Position(Pos), m_AttenuationFactors(AttenuationFactors)
	{
	}

	void PointLight::Update(float DeltaTime)
	{
		Light::Update(DeltaTime);

		m_LightShadowInfo->LightViewProjMat = m_LightShadowInfo->LightProjMat * gm::View::LookAt(Position, gm::Vector3(0.0f), gm::Vector3::YAxis);
	}

	void PointLight::Enable(Shader& shader, const std::string& LightName) const
	{
		Light::Enable(shader, LightName);
		
		// Set the position and attenuation factors for the light
		shader.SetUniform3f((LightName + ".Position").c_str(), Position);
		shader.SetUniform3f((LightName + ".AttenuationFactors").c_str(), m_AttenuationFactors);
	}

	void PointLight::Disable() const
	{
		Light::Disable();
	}

	PointLight::~PointLight()
	{
	}
}