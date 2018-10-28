#pragma once
#include <iostream>

namespace gm
{
	// Forward Declaration
	struct Vector4;

	class Matrix
	{
	public:
		// 2D array to represent the matrix 
		float M[4][4];

		/* Identity Matrix */
		static const Matrix Identity;

		/* Returns an identity matrix */
		static Matrix GetIdentity();

		/* Default matrix */
		explicit Matrix();

		/* Initialise the matrix with Value */
		explicit Matrix(float Value);

		/* Copy Contructor */
		Matrix(const Matrix& OtherMat);

	public:
		/* Returns whether the matrix is equal to this one */
		bool operator==(const Matrix& OtherMat) const;

		/* Returns whether the matrix is not equal to this one */
		bool operator!=(const Matrix& OtherMat) const;

		/* Returns a matrix obtained after adding the matrix to this one */
		const Matrix operator+(const Matrix& OtherMat) const;

		/* Returns a matrix obtained after subtracting the matrix from this one */
		const Matrix operator-(const Matrix& OtherMat) const;

		/* Returns a matrix after multiplying the matrix with this one */
		const Matrix operator*(const Matrix& OtherMat) const;

		/* Returns the matrix obtained after adding value to this matrix */
		const Matrix operator+(float Value) const;

		/* Returns the matrix obtained after subtracting value from this matrix */
		const Matrix operator-(float Value) const;

		/* Returns the matrix obtained after multiplying value with this matrix */
		const Matrix operator*(float Value) const;

		/********* Assignment operators ***********/

		/* Adds the matrix to this matrix */
		Matrix& operator+=(const Matrix& OtherMat);

		/* Subtracts the matrix to this matrix */
		Matrix& operator-=(const Matrix& OtherMat);

		/* Multiply matrix to this matrix */
		Matrix& operator*=(const Matrix& OtherMat);

		/* Adds the value to this matrix */
		Matrix& operator+=(float Value);

		/* Subtracts the value from this matrix */
		Matrix& operator-=(float Value);

		/* Multiplies value with this matrix */
		Matrix& operator*=(float Value);

	public:
		/* Converts the matrix into an identity matrix */
		void MakeIdentity();

		/* Returns a transpose matrix of this one */
		Matrix GetTranspose() const;

		/* Returns the vector obtained after multipying a vec4 with this matrix */
		const Vector4 operator*(const Vector4& Vec) const;

	private:
		/* Initialise the matrix with Value */
		void Init(float Value);
	};

}