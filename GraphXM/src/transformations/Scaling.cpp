#include "Scaling.h"

#include "../vectors/Vector2.h"
#include "../vectors/Vector3.h"

#include "../matrices/Matrix3.h"
#include "../matrices/Matrix4.h"

namespace gm
{
	Matrix4 Scaling::Scale(const Matrix4& Mat, const Vector3& Vec)
	{
		Matrix4 result = GetScaleMatrix(Vec);

		return Mat * result;
	}

	Matrix4 Scaling::Scale(const Matrix4& Mat, float Value)
	{
		Matrix4 result = GetScaleMatrix(Vector3(Value));

		return Mat * result;
	}

	Matrix3 Scaling::Scale(const Matrix3& Mat, const Vector2& Vec)
	{
		Matrix3 result = GetScaleMatrix(Vec);

		return Mat * result;
	}

	Matrix3 Scaling::Scale(const Matrix3& Mat, float Value)
	{
		Matrix3 result = GetScaleMatrix(Vector2(Value));

		return Mat * result;
	}

	Matrix4 Scaling::GetScaleMatrix(const Vector3& Vec)
	{
		Matrix4 result;
		result.M[0][0] = Vec.x;
		result.M[1][1] = Vec.y;
		result.M[2][2] = Vec.z;

		return result;
	}

	Matrix3 Scaling::GetScaleMatrix(const Vector2& Vec)
	{
		Matrix3 result;
		result.M[0][0] = Vec.x;
		result.M[1][1] = Vec.y;

		return result;
	}
}