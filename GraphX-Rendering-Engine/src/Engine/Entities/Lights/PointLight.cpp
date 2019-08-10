#include "pch.h"

#include "PointLight.h"
#include "Shaders/Shader.h"
#include "Entities/Camera.h"

namespace GraphX
{
	PointLight::PointLight(const GraphXMaths::Vector3& Pos, const GraphXMaths::Vector4& Col, float Intensity, const GraphXMaths::Vector3& AttenuationFactors)
		: Light(Col, GraphXMaths::Projection::Ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 1000.0f), Intensity), Position(Pos), m_AttenuationFactors(AttenuationFactors)
	{
	}

	void PointLight::Update(float DeltaTime)
	{
		Light::Update(DeltaTime);

		m_LightShadowInfo->LightViewProjMat = m_LightShadowInfo->LightProjMat * GraphXMaths::View::LookAt(Position, GraphXMaths::Vector3(0.0f), GraphXMaths::Vector3::YAxis);
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