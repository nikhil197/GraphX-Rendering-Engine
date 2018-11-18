#include "Projection.h"

#include "../matrices/Matrix4.h"

#include "../vectors/Vector3.h"

#include "Translation.h"
#include "Scaling.h"
#include "../MathUtil.h"

namespace gm
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

		// Directly add the value to the resultant matrix to avoid the matrix multiplication
		/*Vector3 TranslationVector;
		TranslationVector.x = (right == left) ? 0.0f : -((right + left) / rml);
		TranslationVector.y = (top == bottom) ? 0.0f : -((top + bottom) / tmb);
		TranslationVector.z = (near == far) ? 0.0f : -((far + near) / fmn);

		Vector3 ScaleVector;
		ScaleVector.x = (right == left) ? right : 2 / rml;
		ScaleVector.y = (top == bottom) ? top : 2 / tmb;
		ScaleVector.z = (far == near) ? far : -2 / fmn;

		Translation trans(TranslationVector);

		Scaling scale(ScaleVector);

		return trans * scale;*/

		return result;
	}

	Matrix4 Projection::Perspective(float left, float right, float bottom, float top, float near, float far)
	{
		Matrix4 result;
		result[0][0] = 2 * near / (right - left);
		result[0][2] = -(right + left) / (right - left);

		result[1][1] = 2 * near / (top - bottom);
		result[1][2] = -(top + bottom) / (top - bottom);

		result[2][2] = (far + near) / (far - near);
		result[2][3] = -2 * far * near / (far - near);

		result[3][2] = 1;
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
		float top = (float)MathUtil::Tan(FieldOfView / 2) * near;
		float right = top * AspectRatio;
		float left = -right;
		float bottom = -top;

		return Perspective(left, right, bottom, top, near, far);
	}
}