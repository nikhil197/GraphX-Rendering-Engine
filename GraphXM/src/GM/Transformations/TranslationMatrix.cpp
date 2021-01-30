#include "GMPch.h"
#include "TranslationMatrix.h"

namespace GM
{
	TranslationMatrix::TranslationMatrix(float Value)
		:Matrix4(),
		m_TranslationVector({Value, Value, Value})
	{
		M[0][3] = M[1][3] = M[2][3] = Value;
	}

	TranslationMatrix::TranslationMatrix(const Vector3& Vec)
		:Matrix4(),
		m_TranslationVector(Vec)
	{
		M[0][3] = Vec.x;
		M[1][3] = Vec.y;
		M[2][3] = Vec.z;
	}

	const TranslationMatrix& TranslationMatrix::operator=(const Matrix4& OtherMat)
	{
		Matrix4::operator=(OtherMat);
		return *this;
	}

	Matrix4 TranslationMatrix::Inverse() const
	{
		// Inverse of the translation matrix is the translation in the other direction
		Vector3 NewTranslationVector(-m_TranslationVector);
		return TranslationMatrix(NewTranslationVector);
	}

	Matrix4 TranslationMatrix::Translate(const Matrix4& Mat, const Vector3& Vec)
	{
		TranslationMatrix result(Vec);

		return Mat * result;
	}

	Matrix4 TranslationMatrix::Translate(const Matrix4& Mat, float Value)
	{
		TranslationMatrix result(Value);

		return Mat * result;
	}
}