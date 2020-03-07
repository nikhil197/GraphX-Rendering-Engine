#include "GMPch.h"
#include "Projection.h"

#include "Matrices/Matrix4.h"
#include "MathUtility.h"

namespace GM
{
	Matrix4 Projection::Ortho(float left, float right, float bottom, float top)
	{
		Matrix4 result;

		Ortho(result, left, right, bottom, top);

		return result;
	}

	void Projection::Ortho(Matrix4& Mat, float left, float right, float bottom, float top)
	{
		// Scale the matrix
		Mat(0, 0) = (right == left) ? right : 2 / (right - left);
		Mat(1, 1) = (top == bottom) ? top : 2 / (top - bottom);
		Mat(2, 2) = 1;

		// Translate
		Mat(0, 3) = (right == left) ? 0.0f : -((right + left) / (right - left));
		Mat(1, 3) = (top == bottom) ? 0.0f : -((top + bottom) / (top - bottom));
		Mat(2, 3) = 0.0f;
	}

	Matrix4 Projection::Ortho(float left, float right, float bottom, float top, float near, float far)
	{
		Matrix4 result;

		Ortho(result, left, right, bottom, top, near, far);

		return result;
	}

	void Projection::Ortho(Matrix4& Mat, float left, float right, float bottom, float top, float near, float far)
	{
		// Scale the matrix
		Mat(0, 0) = (right == left) ? right : 2 / (right - left);
		Mat(1, 1) = (top == bottom) ? top : 2 / (top - bottom);
		Mat(2, 2) = (far == near) ? far : -2 / (far - near);

		// Translate
		Mat(0, 3) = (right == left) ? 0.0f : -((right + left) / (right - left));
		Mat(1, 3) = (top == bottom) ? 0.0f : -((top + bottom) / (top - bottom));
		Mat(2, 3) = (near == far) ? 0.0f : -((far + near) / (far - near));
	}

	Matrix4 Projection::Frustum(float left, float right, float bottom, float top, float near, float far)
	{
		Matrix4 result;
		
		Frustum(result, left, right, bottom, top, near, far);

		return result;
	}

	void Projection::Frustum(Matrix4& Mat, float left, float right, float bottom, float top, float near, float far)
	{
		Mat(0, 0) = 2 * near / (right - left);
		Mat(0, 2) = (right + left) / (right - left);
		
		Mat(1, 1) = 2 * near / (top - bottom);
		Mat(1, 2) = (top + bottom) / (top - bottom);
		
		Mat(2, 2) = -(far + near) / (far - near);
		Mat(2, 3) = -2 * far * near / (far - near);

		Mat(3, 2) = -1;
		Mat(3, 3) = 0;
	}

	Matrix4 Projection::Perspective(float FieldOfView, float width, float height, float near, float far)
	{
		return Perspective(FieldOfView, width / height, near, far);
	}

	void Projection::Perspective(Matrix4& Mat, float FieldOfView, float width, float height, float near, float far)
	{
		return Perspective(Mat, FieldOfView, width / height, near, far);
	}

	Matrix4 Projection::Perspective(float FieldOfView, float AspectRatio, float near, float far)
	{
		Matrix4 result;

		Perspective(result, FieldOfView, AspectRatio, near, far);

		return result;
	}

	void Projection::Perspective(Matrix4& Mat, float FieldOfView, float AspectRatio, float near, float far)
	{
		// Calculate the dimensions of the bounding frustum
		float tanOfFov = (float)Utility::Tan(FieldOfView / 2);

		Mat(0, 0) = 1 / (AspectRatio * tanOfFov);
		Mat(1, 1) = 1 / tanOfFov;
		Mat(2, 2) = -(far + near) / (far - near);
		Mat(2, 3) = -2 * near * far / (far - near);

		Mat(3, 2) = -1;
		Mat(3, 3) = 0;
	}
}