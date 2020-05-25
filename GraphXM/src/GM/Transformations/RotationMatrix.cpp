#include "GMPch.h"
#include "RotationMatrix.h"

#include "MathUtility.h"
#include "RotationTranslationMatrix.h"
#include "Misc/Rotator.h"

namespace GM
{
	RotationMatrix::RotationMatrix(float Value)
		:Matrix4(),
		m_Rotation(Value)
	{
		M[0][0] =  Utility::Cos(Value);
		M[0][1] = -Utility::Sin(Value);
		M[1][0] =  Utility::Sin(Value);
		M[1][1] =  Utility::Cos(Value);
	}

	RotationMatrix::RotationMatrix(const Vector3& Angles)
		: Matrix4(),
		m_Rotation(Angles.x, Angles.y, Angles.z)
	{
		RotationTranslationMatrix::Make(*this, m_Rotation, Vector3::ZeroVector);
	}

	RotationMatrix::RotationMatrix(const Rotator& Rot)
	{
		RotationTranslationMatrix::Make(*this, m_Rotation, Vector3::ZeroVector);
	}

	RotationMatrix::RotationMatrix(float Angle, const Vector3& Axis)
	{
		RotationTranslationMatrix::Make(*this, Angle, Axis, Vector3::ZeroVector);
	}

	const RotationMatrix& RotationMatrix::operator=(const Matrix4& OtherMat)
	{
		Matrix4::operator=(OtherMat);
		return *this;
	}

	Matrix4 RotationMatrix::Inverse() const
	{
		// For rotation the inverse is the transpose of the matrix
		return Transpose();
	}

	/* Static Members */
	Matrix4 RotationMatrix::Rotate(const Matrix4& Mat, const Vector3& Angles)
	{
		RotationMatrix result(Angles);

		return Mat * result;
	}

	Matrix4 RotationMatrix::Rotate(const Matrix4& Mat, float Angle, const Vector3& Axis)
	{
		return Mat * RotationMatrix(Angle, Axis);
	}

	Matrix4 RotationMatrix::Rotate(const Matrix4& Mat, float Value)
	{
		RotationMatrix result(Value);

		return Mat * result;
	}
}