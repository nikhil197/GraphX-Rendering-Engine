#pragma once

#include "Entities/Entity.h"

namespace engine
{
	class Light
		: public Entity
	{
	public:
		/* The position of the light */
		gm::Vector3 Position;

		/* Color of the light */
		gm::Vector4 Color;

		/* Intensity of the light (total energy emitted by the light) */
		float Intensity;

		/* Whether to show the details UI window (ImGUI) or not */
		bool bShowDetails : 1;

	protected:
		/* Combined View and projection matrix from the perspective of the light */
		gm::Matrix4 m_LightViewProjMat;

		/* Projection matrix from the perspective of the light */
		gm::Matrix4 m_LightProjMatrix;

	protected:
		/* Constructor */
		Light(const gm::Vector3& Pos, const gm::Vector4& Color, gm::Matrix4 ProjMat, float Intensity = 1.0f);

	public:
		/* Updates the status of the light */
		virtual void Update(float DeltaTime) override;

		/* Enable the light for rendering (Light Name is the name of the light used in shader) */
		virtual void Enable(class Shader& shader, const std::string& LightName = "u_Light") const override;

		/* Disable the light after rendering is done */
		virtual void Disable() const override;

		/* Sets the light view and projection matrices for the purpose of light */
		void SetLightSpaceMatrix(class Shader& DepthShader, const std::string& LightName = "u_Light") const;

		/* Returns the View, projection matrix from the perspective of light */
		const gm::Matrix4& GetLightSpaceMatrix() const { return m_LightViewProjMat; }

		/* Destructor */
		virtual ~Light();
	};
}