#pragma once

namespace gm
{
	// Forward declarations
	struct Vector2;
	struct Vector3;

	class Matrix3;
	class Matrix4;

	class Scaling
	{
	private:
		/* To make sure that no instance of this class is created */
		Scaling();

	public:
		/* Returns a 4x4 matrix obtained after scaling mat with given scale vector */
		static Matrix4 Scale(const Matrix4& Mat, const Vector3& Vec);

		/* Returns a 4x4 matrix obtained after scaling mat with given Value */
		static Matrix4 Scale(const Matrix4& Mat, float Value);

		/* Returns a 3x3 matrix obtained after scaling mat with given scale vector */
		static Matrix3 Scale(const Matrix3& Mat, const Vector2& Vec);

		/* Returns a  3x3 matrix obtained after scaling mat with given Value */
		static Matrix3 Scale(const Matrix3& Mat, float Value);

		/* Returns a 4x4 scale matrix representing the linear scale transformation */
		static Matrix4 GetScaleMatrix(const Vector3& Vec);

		/* Returns a 3x3 scale matrix representing the linear scale transformation */
		static Matrix3 GetScaleMatrix(const Vector2& Vec);
	};
}