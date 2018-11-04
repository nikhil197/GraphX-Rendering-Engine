#pragma once

namespace gm
{
	// Forward declaration
	struct Vector2;
	struct Vector3;

	class Matrix3;
	class Matrix4;

	class Rotation
	{
	private:
		Rotation();

	public:
		/* Returns a 4x4 matrix obtained after rotation mat with given angles vector */
		static Matrix4 Rotate(const Matrix4& Mat, const Vector3& Angles);

		/* Returns a 4x4 matrix obtained after rotating mat with given angles vector about the given axis */
		static Matrix4 Rotate(const Matrix4& Mat, const Vector3& Angles, const Vector3& Axis);

		/* Returns a 4x4 matrix obtained after rotation mat with given angle */
		static Matrix4 Rotate(const Matrix4& Mat, float Angle);

		/* Returns a 3x3 matrix obtained after rotation mat with given angle about the z-axis */
		static Matrix3 Rotate(const Matrix3& Mat, float Angle);

		/* Returns a 4x4 matrix representing the rotation linear transformation with angles */
		static Matrix4 GetRotationMatrix(const Vector3& Angles);

		/* Returns a 4x4 matrix representing the rotation about the given axis */
		static Matrix4 GetRotationMatrix(const Vector3& Angles, const Vector3& Axis);

		/* Returns a 3x3 matrix representing the rotation linear transformation with angle (In 2D, rotation is about z-axis only) */
		static Matrix3 GetRotationMatrix(float Value);
	};
}