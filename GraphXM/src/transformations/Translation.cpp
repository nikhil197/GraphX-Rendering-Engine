#include "Translation.h"

#include "../matrices/Matrix3.h"
#include "../matrices/Matrix4.h"
#include "../vectors/Vector2.h"
#include "../vectors/Vector3.h"

namespace gm
{
	Matrix4 Translation::Translate(const Matrix4& Mat, const Vector3& Vec)
	{
		Matrix4 result = GetTranslationMatrix(Vec);

		return Mat * result;
	}

	Matrix4 Translation::Translate(const Matrix4& Mat, float Value)
	{
		Matrix4 result = GetTranslationMatrix(Vector3(Value));

		return Mat * result;
	}

	Matrix3 Translation::Translate(const Matrix3& Mat, const Vector2& Vec)
	{
		Matrix3 result = GetTranslationMatrix(Vec);

		return Mat * result;
	}

	Matrix3 Translation::Translate(const Matrix3& Mat, float Value)
	{
		Matrix3 result = GetTranslationMatrix(Vector2(Value));

		return Mat * result;
	}

	Matrix4 Translation::GetTranslationMatrix(const Vector3& Vec)
	{
		Matrix4 result;
		result.M[0][3] = Vec.x;
		result.M[1][3] = Vec.y;
		result.M[2][3] = Vec.z;

		return result;
	}

	Matrix3 Translation::GetTranslationMatrix(const Vector2& Vec)
	{
		Matrix3 result;
		result.M[0][2] = Vec.x;
		result.M[1][2] = Vec.y;

		return result;
	}
}