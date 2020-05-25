#pragma once

namespace GraphX
{
	namespace EngineConstants
	{
		// GravityValue value used for the engine
		const float GravityValue = -9.8f;

		/* Far plane of the engine main camera */
		const float FarPlane = 5000.0f;

		/* Near plane of the engine main camera */
		const float NearPlane = 0.1f;

		/* Texture slot used for the shadow map */
		const uint32_t ShadowMapTextureSlot = 5;

		/****** Six Directions ******/
		/* Forward Axis for the engine */
		const GM::Vector3 ForwardAxis(0.0f, 0.0f, -1.0f);

		/* Backward Axis for the engine */
		const GM::Vector3 BackwardAxis(0.0f, 0.0f, 1.0f);

		/* Right Axis for the engine */
		const GM::Vector3 RightAxis(1.0f, 0.0f, 0.0f);

		/* Left Axis for the engine */
		const GM::Vector3 LeftAxis(-1.0f, 0.0f, 0.0f);

		/* Up Axis for the engine */
		const GM::Vector3 UpAxis(0.0f, 1.0f, 0.0f);

		/* Down Axis for the engine */
		const GM::Vector3 DownAxis(0.0f, -1.0f, 0.0f);
	};
}