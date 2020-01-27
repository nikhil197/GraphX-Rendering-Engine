#pragma once

#include "Vectors/Vector3.h"
#include "Matrices/Matrix4.h"

namespace GM
{
	class Scaling
		: public Matrix4
	{
	private:
		/* Vector to store the scale for the matrix w.r.t each axis */
		Vector3 m_ScaleVector;

	public:
		/* Construct a scaling matrix to scale by value in all directions */
		explicit Scaling(float Value);

		/* Construct a scaling matrix to scale by given vector */
		explicit Scaling(const Vector3& Vec);

	public:
		/* Assignment Operator */
		const Scaling& operator=(const Matrix4& OtherMat);

		/* Inverse of the scale matrix */
		Matrix4 Inverse() const override;

		/* Returns the scale for the matrix */
		inline const Vector3& GetScaleVector() const { return m_ScaleVector; }
	public:
		/* Static Members */
		/* Returns a 4x4 matrix obtained after scaling mat with given scale vector */
		static Matrix4 Scale(const Matrix4& Mat, const Vector3& Vec);

		/* Returns a 4x4 matrix obtained after scaling mat with given Value */
		static Matrix4 Scale(const Matrix4& Mat, float Value);
	};
}