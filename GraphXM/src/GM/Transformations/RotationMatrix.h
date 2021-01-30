#pragma once

#include "Matrices/Matrix4.h"
#include "Vectors/Vector3.h"
#include "Misc/Rotator.h"

namespace GM
{
	class RotationMatrix
		: public Matrix4
	{
	private:
		/* Vector to store the angles */
		Rotator m_Rotation;

	public:
		/* Construct a Rotation matrix using the value as Angles for rotation about z-axis */
		explicit RotationMatrix(float Value);

		/* Construct a Rotation matrix to rotate specified angles using the Angles */
		explicit RotationMatrix(const Vector3& Angles);

		/* Construct a rotation matrix from the given rotator */
		explicit RotationMatrix(const Rotator& Rot);

		/* Construct a Rotation matrix to rotate specified angle about an axis */
		RotationMatrix(float Angle, const Vector3& Axis);

	public:
		/* Assignment operator */
		const RotationMatrix& operator=(const Matrix4& OtherMat);

		/* Member functions */
		Matrix4 Inverse() const override;

		/* Returns the angles */
		inline const Rotator& GetRotation() const { return m_Rotation; }

		/* Static Members */
		/* Returns a 4x4 matrix obtained after rotation mat with given angles vector */
		static Matrix4 Rotate(const Matrix4& Mat, const Vector3& Angles);

		/* Returns a 4x4 matrix obtained after rotating mat with given angles vector about the given axis */
		static Matrix4 Rotate(const Matrix4& Mat, float Angle, const Vector3& Axis);

		/* Returns a 4x4 matrix obtained after rotation mat with given angle */
		static Matrix4 Rotate(const Matrix4& Mat, float Angle);
	};
}