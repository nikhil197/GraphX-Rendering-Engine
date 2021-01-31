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

		/* Slot used to bind the skybox for rendering */
		const uint32_t SkyboxBindingSlot = 1;

		/* Texture slot used for the shadow map */
		const uint32_t ShadowMapTextureSlot = 5;

		/****** Six Directions ******/
		/* Forward Axis for the engine */
		const GM::Vector3 ForwardAxis{ 1.0f, 0.0f, 0.0f };

		/* Backward Axis for the engine */
		const GM::Vector3 BackwardAxis{ -1.0f, 0.0f, 0.0f };

		/* Right Axis for the engine */
		const GM::Vector3 RightAxis{ 0.0f, -1.0f, 0.0f };

		/* Left Axis for the engine */
		const GM::Vector3 LeftAxis{ 0.0f, 1.0f, 0.0f };

		/* Up Axis for the engine */
		const GM::Vector3 UpAxis{ 0.0f, 0.0f, 1.0f };

		/* Down Axis for the engine */
		const GM::Vector3 DownAxis{ 0.0f, 0.0f, -1.0f };

		/* Offset To be added to the rotation of objects in order to rotate the Co ordinate axes so that z - axis is Up and x - axis  is forward */
		const GM::Rotator AxesTransformRotationOffset{ 0.0f, -90.0f, 90.0f };

		// Multi threading constants

		/* Number of threads in the global thread pool */
		const uint32_t GThreadPoolThreadCount = 4;

		/* Wait time for a queued thread (in Milliseconds) */
		const uint32_t QueuedThreadWaitTime = 10;
	};
}