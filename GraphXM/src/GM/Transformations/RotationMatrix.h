#pragma once

#include "Matrices/Matrix4.h"
#include "Vectors/Vector3.h"

namespace GM
{
	class RotationMatrix
		: public Matrix4
	{
	private:
		/* Vector to store the angles */
		Vector3 m_Angles;

	public:
		/* Construct a Rotation matrix using the value as Angles for rotation about z-axis */
		explicit RotationMatrix(float Value);

		/* Construct a Rotation matrix to rotate specified angles using the Angles */
		explicit RotationMatrix(const Vector3& Angles);

		/* Construct a Rotation matrix to rotate specified angle about an axis */
		RotationMatrix(float Angle, const Vector3& Axis);

	public:
		/* Assignment operator */
		const RotationMatrix& operator=(const Matrix4& OtherMat);

		/* Member functions */
		Matrix4 Inverse() const override;

		/* Returns the angles */
		inline const Vector3& GetAngles() const { return m_Angles; }

		/* Static Members */
		/* Returns a 4x4 matrix obtained after rotation mat with given angles vector */
		static Matrix4 Rotate(const Matrix4& Mat, const Vector3& Angles);

		/* Returns a 4x4 matrix obtained after rotating mat with given angles vector about the given axis */
		static Matrix4 Rotate(const Matrix4& Mat, float Angle, const Vector3& Axis);

		/* Returns a 4x4 matrix obtained after rotation mat with given angle */
		static Matrix4 Rotate(const Matrix4& Mat, float Angle);
	};
}