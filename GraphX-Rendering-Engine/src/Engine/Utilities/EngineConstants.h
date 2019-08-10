#pragma once

namespace GraphX
{
	// Gravity value used for the engine
	const float GX_ENGINE_GRAVITY = -9.8f;

	/* Far plane of the engine main camera */
	const float GX_ENGINE_FAR_PLANE = 5000.0f;

	/* Near plane of the engine main camera */
	const float GX_ENGINE_NEAR_PLANE = 0.1f;

	/* Texture slot used for the shadow map */
	const unsigned int GX_ENGINE_SHADOW_MAP_TEXTURE_SLOT = 5;
}