#include "Scaling.h"

#include "../vectors/Vector3.h"

#include "../matrices/Matrix4.h"

namespace GraphXMaths
{
	Scaling::Scaling(float Value)
		:Matrix4(),
		m_ScaleVector({Value, Value, Value})
	{
		M[0][0] = M[1][1] = M[2][2] = Value;
	}

	Scaling::Scaling(const Vector3& Vec)
		:Matrix4(),
		m_ScaleVector(Vec)
	{
		M[0][0] = Vec.x;
		M[1][1] = Vec.y;
		M[2][2] = Vec.z;
	}

	const Scaling& Scaling::operator=(const Matrix4& OtherMat)
	{
		Matrix4::operator=(OtherMat);
		return *this;
	}

	Matrix4 Scaling::Inverse() const
	{
		Vector3 NewScaleVector(m_ScaleVector.Reciprocal());
		return Scaling(NewScaleVector);
	}

	Matrix4 Scaling::Scale(const Matrix4& Mat, const Vector3& Vec)
	{
		Scaling result(Vec);

		return Mat * result;
	}

	Matrix4 Scaling::Scale(const Matrix4& Mat, float Value)
	{
		Scaling result(Value);

		return Mat * result;
	}
}