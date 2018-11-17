#include "Translation.h"

#include "../matrices/Matrix4.h"

#include "../vectors/Vector3.h"

namespace gm
{
	Translation::Translation(float Value)
		:Matrix4(),
		m_TranslationVector({Value, Value, Value})
	{
		M[0][3] = M[1][3] = M[2][3] = Value;
	}

	Translation::Translation(const Vector3& Vec)
		:Matrix4(),
		m_TranslationVector(Vec)
	{
		M[0][3] = Vec.x;
		M[1][3] = Vec.y;
		M[2][3] = Vec.z;
	}

	const Translation& Translation::operator=(const Matrix4& OtherMat)
	{
		Matrix4::operator=(OtherMat);
		return *this;
	}

	Matrix4 Translation::Inverse() const
	{
		// Inverse of the translation matrix is the translation in the other direction
		Vector3 NewTranslationVector(-m_TranslationVector);
		return Translation(NewTranslationVector);
	}

	Matrix4 Translation::Translate(const Matrix4& Mat, const Vector3& Vec)
	{
		Translation result(Vec);

		return Mat * result;
	}

	Matrix4 Translation::Translate(const Matrix4& Mat, float Value)
	{
		Translation result(Value);

		return Mat * result;
	}
}