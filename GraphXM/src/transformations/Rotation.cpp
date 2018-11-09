#include "Rotation.h"
#include "Translation.h"

#include "../vectors/Vector2.h"
#include "../vectors/Vector3.h"

#include "../matrices/Matrix3.h"
#include "../matrices/Matrix4.h"

#include "../MathUtil.h"

namespace gm
{
	Rotation::Rotation(float Value)
		:Matrix4(),
		m_Angles(Vector3(Value))
	{
		M[0][0] =  (float)MathUtil::Cos(Value);
		M[0][1] = -(float)MathUtil::Sin(Value);
		M[1][0] =  (float)MathUtil::Sin(Value);
		M[1][1] =  (float)MathUtil::Cos(Value);
	}

	Rotation::Rotation(const Vector3& Angles)
		: Matrix4(),
		m_Angles(Angles)
	{
		Matrix4 resultX;
		Matrix4 resultY;
		Matrix4 resultZ;

		// Rotating about x - axis
		resultX[1][1] =  (float)MathUtil::Cos(Angles.x);
		resultX[2][2] =  (float)MathUtil::Cos(Angles.x);
		resultX[1][2] = -(float)MathUtil::Sin(Angles.x);
		resultX[2][1] =  (float)MathUtil::Sin(Angles.x);

		// Rotating about y - axis
		resultY[0][0] =  (float)MathUtil::Cos(Angles.y);
		resultY[0][2] =  (float)MathUtil::Sin(Angles.y);
		resultY[2][2] =  (float)MathUtil::Cos(Angles.y);
		resultY[2][0] = -(float)MathUtil::Sin(Angles.y);

		// Rotating about z - axis
		resultZ[0][0] =  (float)MathUtil::Cos(Angles.z);
		resultZ[0][1] = -(float)MathUtil::Sin(Angles.z);
		resultZ[1][0] =  (float)MathUtil::Sin(Angles.z);
		resultZ[1][1] =  (float)MathUtil::Cos(Angles.z);

		*this = resultZ * resultY * resultX;
	}

	//TODO:: Complete this
	Rotation::Rotation(const Vector3& Angles, const Vector3& Axis)
	{

	}

	const Rotation& Rotation::operator=(const Matrix4& OtherMat)
	{
		Matrix4::operator=(OtherMat);
		return *this;
	}

	Matrix4 Rotation::Inverse() const
	{
		// For rotation the inverse is the transpose of the matrix
		return Transpose();
	}

	/* Static Members */
	Matrix4 Rotation::Rotate(const Matrix4& Mat, const Vector3& Angles)
	{
		Rotation result(Angles);

		return Mat * result;
	}

	// TODO: To Be implemented
	Matrix4 Rotation::Rotate(const Matrix4& Mat, const Vector3& Angles, const Vector3& Axis)
	{
		return Matrix4();
	}

	Matrix4 Rotation::Rotate(const Matrix4& Mat, float Value)
	{
		Rotation result(Value);

		return Mat * result;
	}
}