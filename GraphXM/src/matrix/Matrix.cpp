#include "Matrix.h"

#include "../vectors/Vector4.h"

namespace gm
{
	const Matrix Matrix::Identity(Matrix::GetIdentity());

	Matrix::Matrix()
	{
		Init(0);
	}

	Matrix::Matrix(float Value)
	{
		Init(Value);
	}

	Matrix Matrix::GetIdentity()
	{
		Matrix result;
		result.MakeIdentity();
		return result;
	}

	Matrix::Matrix(const Matrix& OtherMat)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				M[i][j] = OtherMat.M[i][j];
			}
		}
	}

	void Matrix::Init(float Value)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				M[i][j] = Value;
			}
		}
	}

	void Matrix::MakeIdentity()
	{
		M[0][0] = 1, M[0][1] = 0, M[0][2] = 0, M[0][3] = 0;
		M[1][0] = 0, M[1][1] = 1, M[1][2] = 0, M[1][3] = 0;
		M[2][0] = 0, M[2][1] = 0, M[2][2] = 1, M[2][3] = 0;
		M[3][0] = 0, M[3][1] = 0, M[3][2] = 0, M[3][3] = 1;
	}

	#pragma region Operators
	bool Matrix::operator==(const Matrix& OtherMat) const
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (M[i][j] != OtherMat.M[i][j])
					return false;
			}
		}

		return true;
	}

	bool Matrix::operator!=(const Matrix& OtherMat) const
	{
		return !(*this == OtherMat);
	}

	const Matrix Matrix::operator+(const Matrix& OtherMat) const
	{
		Matrix result;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result.M[i][j] = M[i][j] + OtherMat.M[i][j];
			}
		}

		return result;
	}

	const Matrix Matrix::operator-(const Matrix& OtherMat) const
	{
		Matrix result;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result.M[i][j] = M[i][j] - OtherMat.M[i][j];
			}
		}

		return result;
	}

	const Matrix Matrix::operator*(const Matrix& OtherMat) const
	{
		Matrix result;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				float val = 0;

				for (int k = 0; k < 4; k++)
				{
					val += M[i][k] * OtherMat.M[k][j];
				}
				result.M[i][j] = val;
			}
		}

		return result;
	}

	const Matrix Matrix::operator+(float Value) const
	{
		Matrix result;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result.M[i][j] = M[i][j] + Value;
			}
		}

		return result;
	}

	const Matrix Matrix::operator-(float Value) const
	{
		Matrix result;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result.M[i][j] = M[i][j] - Value;
			}
		}

		return result;
	}

	const Matrix Matrix::operator*(float Value) const
	{
		Matrix result;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result.M[i][j] = M[i][j] * Value;
			}
		}

		return result;
	}

	Matrix& Matrix::operator+=(const Matrix& OtherMat)
	{
		*this = *this + OtherMat;
		return *this;
	}

	Matrix& Matrix::operator-=(const Matrix& OtherMat)
	{
		*this = *this - OtherMat;
		return *this;
	}

	Matrix& Matrix::operator*=(const Matrix& OtherMat)
	{
		*this = *this * OtherMat;
		return *this;
	}

	Matrix& Matrix::operator+=(float Value)
	{
		*this = *this + Value;
		return *this;
	}

	Matrix& Matrix::operator-=(float Value)
	{
		*this = *this - Value;
		return *this;
	}

	Matrix& Matrix::operator*=(float Value)
	{
		*this = *this * Value;
		return *this;
	}
	#pragma endregion

	Matrix Matrix::GetTranspose() const
	{
		Matrix result;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result.M[j][i] = M[i][j];
			}
		}

		return result;
	}

	const Vector4 Matrix::operator*(const Vector4& Vec) const
	{
		Vector4 result(
			M[0][0] * Vec.x + M[0][1] * Vec.y + M[0][2] * Vec.z + M[0][3] * Vec.w,
			M[1][0] * Vec.x + M[1][1] * Vec.y + M[1][2] * Vec.z + M[1][3] * Vec.w,
			M[2][0] * Vec.x + M[2][1] * Vec.y + M[2][2] * Vec.z + M[2][3] * Vec.w,
			M[3][0] * Vec.x + M[3][1] * Vec.y + M[3][2] * Vec.z + M[3][3] * Vec.w
		);

		return result;
	}
}