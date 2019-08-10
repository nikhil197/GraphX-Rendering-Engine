#include "CommonPch.h"
#include "Matrix3.h"

#include "Matrix4.h"
#include "Vectors/Vector3.h"

namespace GraphXMaths
{
	Matrix3::Matrix3()
	{
		Identity();
	}

	Matrix3::Matrix3(float Value)
	{
		Init(Value);
	}

	Matrix3::Matrix3(const float(*arr)[3])
	{
		Init(arr);
	}

	Matrix3::Matrix3(const Matrix4& OtherMat)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				M[i][j] = OtherMat[i][j];
			}
		}
	}

	Matrix3::Matrix3(const Matrix3& OtherMat)
	{
		Init(OtherMat.M);
	}

	void Matrix3::Init(float Value)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				M[i][j] = Value;
			}
		}
	}

	void Matrix3::Init(const float(*arr)[3])
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				M[i][j] = arr[i][j];
			}
		}
	}

	void Matrix3::Identity()
	{
		M[0][0] = 1, M[0][1] = 0, M[0][2] = 0;
		M[1][0] = 0, M[1][1] = 1, M[1][2] = 0;
		M[2][0] = 0, M[2][1] = 0, M[2][2] = 1;
	}

	#pragma region Operators
	Matrix3& Matrix3::operator=(const Matrix3& OtherMat)
	{
		Init(OtherMat.M);

		return *this;
	}

	bool Matrix3::operator==(const Matrix3& OtherMat) const
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (M[i][j] != OtherMat[i][j])
					return false;
			}
		}

		return true;
	}

	bool Matrix3::operator!=(const Matrix3& OtherMat) const
	{
		return !(*this == OtherMat);
	}

	const Matrix3 Matrix3::operator+(const Matrix3& OtherMat) const
	{
		Matrix3 result;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				result[i][j] = M[i][j] + OtherMat[i][j];
			}
		}

		return result;
	}

	const Matrix3 Matrix3::operator-(const Matrix3& OtherMat) const
	{
		Matrix3 result;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				result[i][j] = M[i][j] - OtherMat[i][j];
			}
		}

		return result;
	}

	const Matrix3 Matrix3::operator*(const Matrix3& OtherMat) const
	{
		Matrix3 result;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				float val = 0;

				for (int k = 0; k < 3; k++)
				{
					val += M[i][k] * OtherMat[k][j];
				}
				result[i][j] = val;
			}
		}

		return result;
	}

	const Matrix3 Matrix3::operator+(float Value) const
	{
		Matrix3 result;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				result[i][j] = M[i][j] + Value;
			}
		}

		return result;
	}

	const Matrix3 Matrix3::operator-(float Value) const
	{
		Matrix3 result;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				result[i][j] = M[i][j] - Value;
			}
		}

		return result;
	}

	const Matrix3 Matrix3::operator*(float Value) const
	{
		Matrix3 result;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				result[i][j] = M[i][j] * Value;
			}
		}

		return result;
	}

	const Matrix3 Matrix3::operator/(float Value) const
	{
		Matrix3 result;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				result[i][j] = (Value == 0) ? FLT_MAX : M[i][j] / Value;
			}
		}

		return result;
	}

	Matrix3& Matrix3::operator+=(const Matrix3& OtherMat)
	{
		*this = *this + OtherMat;
		return *this;
	}

	Matrix3& Matrix3::operator-=(const Matrix3& OtherMat)
	{
		*this = *this - OtherMat;
		return *this;
	}

	Matrix3& Matrix3::operator*=(const Matrix3& OtherMat)
	{
		*this = *this * OtherMat;
		return *this;
	}

	Matrix3& Matrix3::operator+=(float Value)
	{
		*this = *this + Value;
		return *this;
	}

	Matrix3& Matrix3::operator-=(float Value)
	{
		*this = *this - Value;
		return *this;
	}

	Matrix3& Matrix3::operator*=(float Value)
	{
		*this = *this * Value;
		return *this;
	}

	Matrix3& Matrix3::operator/=(float Value)
	{
		*this = *this / Value;
		return *this;
	}

	#pragma endregion

	Matrix3 Matrix3::Transpose() const
	{
		Matrix3 result;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				result[j][i] = M[i][j];
			}
		}

		return result;
	}

	const Vector3 Matrix3::operator*(const Vector3& Vec) const
	{
		Vector3 result(
			M[0][0] * Vec.x + M[0][1] * Vec.y + M[0][2] * Vec.z,
			M[1][0] * Vec.x + M[1][1] * Vec.y + M[1][2] * Vec.z,
			M[2][0] * Vec.x + M[2][1] * Vec.y + M[2][2] * Vec.z
		);

		return result;
	}

	float* const Matrix3::operator[](int index) const
	{
		if (index < 0)
			index = 0;
		else if (index > 3)
			index = 3;

		return (float* const)M[index];
	}

	float Matrix3::Determinant() const
	{
		return (
			+ M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1])
			- M[0][1] * (M[1][0] * M[2][2] - M[1][2] * M[2][0])
			+ M[0][2] * (M[1][0] * M[2][1] - M[1][1] * M[2][0])
			);
	}

	Matrix3 Matrix3::AdjointTranspose() const
	{
		Matrix3 mat;

		// First Row
		mat[0][0] = +(M[1][1] * M[2][2] - M[1][2] * M[2][1]);
		mat[0][1] = -(M[1][0] * M[2][2] - M[1][2] * M[2][0]);
		mat[0][2] = +(M[1][0] * M[2][1] - M[1][1] * M[2][0]);
		
		// Second Row
		mat[1][0] = -(M[0][1] * M[2][2] - M[0][2] * M[2][1]);
		mat[1][1] = +(M[0][0] * M[2][2] - M[0][2] * M[2][0]);
		mat[1][2] = -(M[0][0] * M[2][1] - M[0][1] * M[2][0]);

		// Third Row
		mat[2][0] = +(M[0][1] * M[1][2] - M[0][2] * M[1][1]);
		mat[2][1] = -(M[0][0] * M[1][2] - M[0][2] * M[1][0]);
		mat[2][2] = +(M[0][0] * M[1][1] - M[0][1] * M[1][0]);

		return mat;
	}

	Matrix3 Matrix3::Adjoint() const
	{
		Matrix3 result(AdjointTranspose());

		result = result.Transpose();

		return result;
	}

	Matrix3 Matrix3::Inverse() const
	{
		Matrix3 mat;

		// Get the adjoint of the matrix
		mat = mat.Adjoint();

		// Divide the adjoint matrix by determinant
		mat /= mat.Determinant();
		return mat;
	}

	/* Non Member functions */
	std::ostream& operator<<(std::ostream& Out, const Matrix3& Mat)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
				Out << Mat[i][j] << ' ';

			Out << '\n';
		}

		return Out;
	}
}