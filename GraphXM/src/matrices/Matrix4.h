#pragma once
#include <iostream>

namespace gm
{
	// Forward Declaration
	struct Vector4;

	class Matrix4
	{
	public:
		// 2D array to represent the 4 x 4 matrix 
		float M[4][4];

		/* Default (Identity) matrix */
		explicit Matrix4();

		/* Initialise the matrix with Value */
		explicit Matrix4(float Value);

		/* Copy Contructor */
		Matrix4(const Matrix4& OtherMat);

	public:
		/* Returns whether the matrix is equal to this one */
		bool operator==(const Matrix4& OtherMat) const;

		/* Returns whether the matrix is not equal to this one */
		bool operator!=(const Matrix4& OtherMat) const;

		/* Returns a matrix obtained after adding the matrix to this one */
		const Matrix4 operator+(const Matrix4& OtherMat) const;

		/* Returns a matrix obtained after subtracting the matrix from this one */
		const Matrix4 operator-(const Matrix4& OtherMat) const;

		/* Returns a matrix after multiplying the matrix with this one */
		const Matrix4 operator*(const Matrix4& OtherMat) const;

		/* Returns the matrix obtained after adding value to this matrix */
		const Matrix4 operator+(float Value) const;

		/* Returns the matrix obtained after subtracting value from this matrix */
		const Matrix4 operator-(float Value) const;

		/* Returns the matrix obtained after multiplying value with this matrix */
		const Matrix4 operator*(float Value) const;

		/* Returns the matrix obtained after dividing this matrix with value */
		const Matrix4 operator/(float Value) const;

		/********* Assignment operators ***********/

		/* Adds the matrix to this matrix */
		Matrix4& operator+=(const Matrix4& OtherMat);

		/* Subtracts the matrix to this matrix */
		Matrix4& operator-=(const Matrix4& OtherMat);

		/* Multiply matrix to this matrix */
		Matrix4& operator*=(const Matrix4& OtherMat);

		/* Adds the value to this matrix */
		Matrix4& operator+=(float Value);

		/* Subtracts the value from this matrix */
		Matrix4& operator-=(float Value);

		/* Multiplies value with this matrix */
		Matrix4& operator*=(float Value);

		/* Divides the matrix with value */
		Matrix4& operator/=(float Value);

	public:
		/* Returns the vector obtained after multipying a vec4 with this matrix */
		const Vector4 operator*(const Vector4& Vec) const;

		/* Converts the matrix into an identity matrix */
		void Identity();

		/* Returns a transpose matrix of this one */
		Matrix4 Transpose() const;

		/* Returns the determinant of this matrix */
		float Determinant() const;

		/* Returns a matrix that is the adjoint of this matrix */
		Matrix4 Adjoint() const;

		/* Returns an inverse matrix of this matrix */
		Matrix4 Inverse() const;

	private:
		/* Initialise the matrix with Value */
		void Init(float Value);
	};

	/* Non Member functions */
	std::ostream& operator<<(std::ostream& Out, const Matrix4& Mat);
}