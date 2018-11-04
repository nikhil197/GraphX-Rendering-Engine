#include "Rotation.h"

#include "../vectors/Vector2.h"
#include "../vectors/Vector3.h"

#include "../matrices/Matrix3.h"
#include "../matrices/Matrix4.h"

#include "../MathUtil.h"

namespace gm
{
	Matrix4 Rotation::Rotate(const Matrix4& Mat, const Vector3& Angles)
	{
		Matrix4 result = GetRotationMatrix(Angles);

		return Mat * result;
	}

	Matrix4 Rotation::Rotate(const Matrix4& Mat, float Value)
	{
		Matrix4 result = GetRotationMatrix(Vector3(Value));

		return Mat * result;
	}

	Matrix3 Rotation::Rotate(const Matrix3& Mat, float Value)
	{
		Matrix3 result = GetRotationMatrix(Value);

		return Mat * result;
	}

	Matrix4 Rotation::GetRotationMatrix(const Vector3& Vec)
	{
		Matrix4 resultX;
		Matrix4 resultY;
		Matrix4 resultZ;

		// Rotating about x - axis
		resultX.M[1][1] =  (float)MathUtil::Cos(Vec.x);
		resultX.M[2][2] =  (float)MathUtil::Cos(Vec.x);
		resultX.M[1][2] = -(float)MathUtil::Sin(Vec.x);
		resultX.M[2][1] =  (float)MathUtil::Sin(Vec.x);

		// Rotating about y - axis
		resultY.M[0][0] =  (float)MathUtil::Cos(Vec.y);
		resultY.M[0][2] =  (float)MathUtil::Sin(Vec.y);
		resultY.M[2][2] =  (float)MathUtil::Cos(Vec.y);
		resultY.M[2][0] = -(float)MathUtil::Sin(Vec.y);

		// Rotating about z - axis
		resultZ.M[0][0] =  (float)MathUtil::Cos(Vec.x);
		resultZ.M[0][1] = -(float)MathUtil::Sin(Vec.x);
		resultZ.M[1][0] =  (float)MathUtil::Sin(Vec.x);
		resultZ.M[1][1] =  (float)MathUtil::Cos(Vec.x);

		return resultZ * resultY * resultX;
	}

	Matrix3 Rotation::GetRotationMatrix(float Value)
	{
		Matrix3 result;
		result.M[0][0] =  (float)MathUtil::Cos(Value);
		result.M[0][1] = -(float)MathUtil::Sin(Value);
		result.M[1][0] =  (float)MathUtil::Sin(Value);
		result.M[1][1] =  (float)MathUtil::Cos(Value);

		return result;
	}
}