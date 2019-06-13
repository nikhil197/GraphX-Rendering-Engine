#pragma once

#include "Entities/Entity.h"

namespace engine
{
	/* To store the shadow information for the light */
	struct LightShadowInfo
	{
		/* Projection matrix from the perspective of the light */
		gm::Matrix4 LightProjMat;

		/* Combined View and projection matrix from the perspective of the light */
		gm::Matrix4 LightViewProjMat;

		LightShadowInfo(const gm::Matrix4& ProjMat, const gm::Matrix4& LightViewProjMat)
			: LightProjMat(ProjMat), LightViewProjMat(LightViewProjMat)
		{
		}
	};

	class Light
		: public Entity
	{
	public:
		/* Color of the light */
		gm::Vector4 Color;

		/* Intensity of the light (total energy emitted by the light) */
		float Intensity;

		/* Whether to show the details UI window (ImGUI) or not */
		bool bShowDetails : 1;

	protected:
		/* Light information required for shadows */
		LightShadowInfo* m_LightShadowInfo;

	protected:
		/* Constructor */
		Light(const gm::Vector4& Color, gm::Matrix4 ProjMat, float Intensity = 1.0f);

	public:
		/* Updates the status of the light */
		virtual void Update(float DeltaTime) override;

		/* Enable the light for rendering (Light Name is the name of the light used in shader) */
		virtual void Enable(class Shader& shader, const std::string& LightName = "u_Light") const override;

		/* Disable the light after rendering is done */
		virtual void Disable() const override;

		/* Sets the light view and projection matrices for the purpose of light */
		void SetLightSpaceMatrix(class Shader& DepthShader, const std::string& LightName = "u_Light") const;

		/* Returns the light's shadow information */
		inline const LightShadowInfo* GetShadowInfo() const { return m_LightShadowInfo; }

		/* Destructor */
		virtual ~Light();
	};
}