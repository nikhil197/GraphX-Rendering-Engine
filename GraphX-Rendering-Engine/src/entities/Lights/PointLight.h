#pragma once

#include "Light.h"

namespace engine
{
	class PointLight
		: public Light
	{
	private:
		/* Coefficients of the attenuation equation used for the light
		*	x -> Constant factor of the equation (Must be 1)
		*	y -> Linear Coefficient of the equation
		*	z -> Quadratic Coefficient of the equation
		*/
		gm::Vector3 m_AttenuationFactors;

	public:
		/* Constructor */
		PointLight(const gm::Vector3& Pos, const gm::Vector4& Col, float Intensity = 1.0f, const gm::Vector3& AttenuationFactors = gm::Vector3(1.0f, 0.0f, 0.0f));

		virtual void Update(float DeltaTime) override;

		virtual void Enable(class Shader& shader, const std::string& LightName = "u_PointLight") const override;

		virtual void Disable() const override;

		/* Returns the attenuation factors of the light */
		inline const gm::Vector3& GetAttenuationFactors() const { return m_AttenuationFactors; }

		/* Sets the new factors for the light attenuation */
		inline void SetAttenuationFactors(const gm::Vector3& NewFactors)
		{
			m_AttenuationFactors = NewFactors;
			m_AttenuationFactors.x = 1.0f;
		}

		/* Destructor */
		virtual ~PointLight();
	};
}