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

		/* Initialise the matrix with an array */
		explicit Matrix3(const float(*arr)[3]);

		/* Copy constructor */
		Matrix3(const Matrix3& OtherMat);

	public:
		/* Assignment Operator */
		Matrix3& operator=(const Matrix3& OtherMat);

		/* Returns whether the matrix is equal to this one */
		bool operator==(const Matrix3& OtherMat) const;

		/* Returns whether the matrix is not equal to this one */
		bool operator!=(const Matrix3& OtherMat) const;

		/* Returns a matrix obtained after adding the matrix to this one */
		const Matrix3 operator+(const Matrix3& OtherMat) const;

		/* Returns a matrix obtained after subtracting the matrix from this one */
		const Matrix3 operator-(const Matrix3& OtherMat) const;

		/* Returns a matrix after multiplying the matrix with this one */
		const Matrix3 operator*(const Matrix3& OtherMat) const;

		/* Returns the matrix obtained after adding value to this matrix */
		const Matrix3 operator+(float Value) const;

		/* Returns the matrix obtained after subtracting value from this matrix */
		const Matrix3 operator-(float Value) const;

		/* Returns the matrix obtained after multiplying value with this matrix */
		const Matrix3 operator*(float Value) const;

		/* Returns the matrix obtained after dividing this matrix with value */
		const Matrix3 operator/(float Value) const;

		/********* Assignment operators ***********/

		/* Adds the matrix to this matrix */
		Matrix3& operator+=(const Matrix3& OtherMat);

		/* Subtracts the matrix to this matrix */
		Matrix3& operator-=(const Matrix3& OtherMat);

		/* Multiply matrix to this matrix */
		Matrix3& operator*=(const Matrix3& OtherMat);

		/* Adds the value to this matrix */
		Matrix3& operator+=(float Value);

		/* Subtracts the value from this matrix */
		Matrix3& operator-=(float Value);

		/* Multiplies value with this matrix */
		Matrix3& operator*=(float Value);

		/* Divides the matrix with value */
		Matrix3& operator/=(float Value);

	public:
		/* Returns the vector obtained after multipying a vec4 with this matrix */
		const Vector3 operator*(const Vector3& Vec) const;

		/* Index operator returns a pointer to the row specified by the index */
		float* const operator[](int index) const;

	public:
		/* Converts this matrix into an identiy matrix */
		void Identity();

		/* Returns a transpose matrix of this one */
		Matrix3 Transpose() const;

		/* Returns the determinant of this matrix */
		float Determinant() const;

		/* Returns a matrix that is the adjoint of this matrix */
		Matrix3 Adjoint() const;

		/* Returns an inverse matrix of this matrix */
		Matrix3 Inverse() const;

	private:
		/* Initialise the matrix */
		void Init(float Value);

		/* Initialise the matrix with the contents of an array */
		void Init(const float(*arr)[3]);

	};
}