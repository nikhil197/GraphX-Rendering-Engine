#pragma once

#include "Light.h"

namespace GraphX
{
	class PointLight
		: public Light
	{
	public:
		/* Constructor */
		PointLight(const GM::Vector3& Pos, const GM::Vector4& Col, float Intensity = 1.0f, const GM::Vector3& AttenuationFactors = GM::Vector3(1.0f, 0.0f, 0.0f));

		virtual void Update(float DeltaTime) override;

		virtual void Enable(class Shader& shader, const std::string& LightName = "u_PointLight") const override;

		virtual void Disable() const override;

		/* Returns the attenuation factors of the light */
		inline const GM::Vector3& GetAttenuationFactors() const { return m_AttenuationFactors; }

		/* Sets the new factors for the light attenuation */
		inline void SetAttenuationFactors(const GM::Vector3& NewFactors)
		{
			m_AttenuationFactors = NewFactors;
			m_AttenuationFactors.x = 1.0f;
		}

		/* Destructor */
		virtual ~PointLight();

	public:
		/* The position of the light */
		GM::Vector3 Position;

	private:

		/* Coefficients of the attenuation equation used for the light
		*	x -> Constant factor of the equation (Must be 1)
		*	y -> Linear Coefficient of the equation
		*	z -> Quadratic Coefficient of the equation
		*/
		GM::Vector3 m_AttenuationFactors;
	};
}