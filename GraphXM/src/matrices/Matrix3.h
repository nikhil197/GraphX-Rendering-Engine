#pragma once

namespace gm
{
	struct Vector3;

	class Matrix3
	{
	public:
		// 2D array to represent the 3 x 3 matrix
		float M[3][3];

		/* Default (Identity) matrix */
		explicit Matrix3();

		/* Initialise the matrix with a value */
		explicit Matrix3(float Value);

		/* Copy constructor */
		Matrix3(const Matrix3& OtherMat);

	public:
		/* Converts this matrix into an identiy matrix */
		void Identity();

	private:
		/* Initialise the matrix */
		void Init(float Value);

	};
}