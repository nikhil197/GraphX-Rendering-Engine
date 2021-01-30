#include "GMPch.h"
#include "ScaleMatrix.h"

namespace GM
{
	ScaleMatrix::ScaleMatrix(float Value)
		:Matrix4(),
		m_ScaleVector({Value, Value, Value})
	{
		M[0][0] = M[1][1] = M[2][2] = Value;
	}

	ScaleMatrix::ScaleMatrix(const Vector3& Vec)
		:Matrix4(),
		m_ScaleVector(Vec)
	{
		M[0][0] = Vec.x;
		M[1][1] = Vec.y;
		M[2][2] = Vec.z;
	}

	const ScaleMatrix& ScaleMatrix::operator=(const Matrix4& OtherMat)
	{
		Matrix4::operator=(OtherMat);
		return *this;
	}

	Matrix4 ScaleMatrix::Inverse() const
	{
		Vector3 NewScaleVector(m_ScaleVector.Reciprocal());
		return ScaleMatrix(NewScaleVector);
	}

	Matrix4 ScaleMatrix::Scale(const Matrix4& Mat, const Vector3& Vec)
	{
		ScaleMatrix result(Vec);

		return Mat * result;
	}

	Matrix4 ScaleMatrix::Scale(const Matrix4& Mat, float Value)
	{
		ScaleMatrix result(Value);

		return Mat * result;
	}
}