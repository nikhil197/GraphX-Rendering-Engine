#include "Projection.h"

#include "../matrices/Matrix4.h"

#include "../vectors/Vector3.h"

#include "Translation.h"
#include "Scaling.h"
#include "../MathUtil.h"

namespace GraphXMaths
{
	Matrix4 Projection::Ortho(float left, float right, float bottom, float top, float near, float far)
	{
		Matrix4 result;

		// Scale the matrix
		result[0][0] = (right == left) ? right : 2 / (right - left);
		result[1][1] = (top == bottom) ? top : 2 / (top - bottom);
		result[2][2] = (far == near) ? far : -2 / (far - near);

		// Translate
		result[0][3] = (right == left) ? 0.0f : -((right + left) / (right - left));
		result[1][3] = (top == bottom) ? 0.0f : -((top + bottom) / (top - bottom));
		result[2][3] = (near == far) ? 0.0f : -((far + near) / (far - near));

		return result;
	}

	// TODO: Logic needs to be fixed
	Matrix4 Projection::Frustum(float left, float right, float bottom, float top, float near, float far)
	{
		Matrix4 result;
		result[0][0] = 2 * near / (right - left);
		result[0][2] = (right + left) / (right - left);

		result[1][1] = 2 * near / (top - bottom);
		result[1][2] = (top + bottom) / (top - bottom);

		result[2][2] = -(far + near) / (far - near);
		result[2][3] = -2 * far * near / (far - near);

		result[3][2] = -1;
		result[3][3] = 0;

		return result;
	}

	Matrix4 Projection::Perspective(float FieldOfView, float width, float height, float near, float far)
	{
		return Perspective(FieldOfView, width / height, near, far);
	}

	Matrix4 Projection::Perspective(float FieldOfView, float AspectRatio, float near, float far)
	{
		// Calculate the dimensions of the bounding frustum
		float tanOfFov = (float)MathUtil::Tan(FieldOfView / 2);
	
		Matrix4 result;
		result[0][0] = 1 / (AspectRatio * tanOfFov);
		result[1][1] = 1 / tanOfFov;
		result[2][2] = -(far + near) / (far - near);
		result[2][3] = -2 * near * far / (far - near);

		result[3][3] = 0;
		result[3][2] = -1;

		return result;
	}
}