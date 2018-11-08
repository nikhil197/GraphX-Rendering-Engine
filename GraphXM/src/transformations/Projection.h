#pragma once

namespace gm
{
	// Forward declarations 
	class Matrix4;

	struct Vector3;

	class Projection
	{
	private:
		Projection();

	public:
		/** Creates an orthographic projection matrix which converts the bounding box given by the dimension into a unit cube */
		static Matrix4 Ortho(float top, float left, float botton, float right, float near, float far);
	};
}