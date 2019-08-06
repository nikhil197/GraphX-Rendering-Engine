#pragma once

#include "matrices\Matrix4.h"
#include "vectors\Vector3.h"

namespace GraphXMaths
{
	class Translation
		:public Matrix4
	{
	private:
		/* Vector to store the values by which the matrix translates w.r.t. each axis */
		Vector3 m_TranslationVector;

	public:
		/* Constructs a translation matrix to translate by given Value */
		explicit Translation(float Value);

		/* Constructs a translation matrix to translate by given vector */
		explicit Translation(const Vector3& Vec);
	public:
		/* Assignment operator */
		const Translation& operator=(const Matrix4& OtherMat);

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