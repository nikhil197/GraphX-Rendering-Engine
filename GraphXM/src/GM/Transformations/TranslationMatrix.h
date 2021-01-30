#pragma once

#include "Matrices/Matrix4.h"
#include "Vectors/Vector3.h"

namespace GM
{
	class TranslationMatrix
		:public Matrix4
	{
	private:
		/* Vector to store the values by which the matrix translates w.r.t. each axis */
		Vector3 m_TranslationVector;

	public:
		/* Constructs a translation matrix to translate by given Value */
		explicit TranslationMatrix(float Value);

		/* Constructs a translation matrix to translate by given vector */
		explicit TranslationMatrix(const Vector3& Vec);
	public:
		/* Assignment operator */
		const TranslationMatrix& operator=(const Matrix4& OtherMat);

		/* Returns the inverse of the translation matrix */
		Matrix4 Inverse() const override;

		/* Returns the translation vector of the matrix */
		inline const Vector3& GetTranslationVector() const { return m_TranslationVector; }
	public:
		/* Static Members */
		/* Returns a 4x4 matrix obtained after translating mat with given translation vector */
		static Matrix4 Translate(const Matrix4& Mat, const Vector3& Vec);

		/* Returns a 4x4 matrix obtained after translating mat with given Value */
		static Matrix4 Translate(const Matrix4& Mat, float Value);
	};
}