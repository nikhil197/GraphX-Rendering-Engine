#pragma once

#include "Light.h"

namespace engine
{
	class DirectionalLight
		: public Light
	{
	public:
		/* Direction of the light */
		gm::Vector3 Direction;

		/* Constructor */
		DirectionalLight(const gm::Vector4& Color, const gm::Vector3& LightDir, float Intensity = 1.0f);

		virtual void Update(float DeltaTime) override;

		virtual void Enable(class Shader& shader, const std::string& LightName = "u_DirectionalLight") const override;

		virtual void Disable() const override;
	};
}