#include "GMPch.h"
#include "Matrix4.h"

#include "Vectors/Vector4.h"

#include "MathUtility.h"

namespace GM
{
	Matrix4::Matrix4()
	{
		Identity();
	}

	Matrix4::Matrix4(float Value)
	{
		Init(Value);
	}

	Matrix4::Matrix4(const float(*arr)[4])
	{
		Init(arr);
	}

	Matrix4::Matrix4(const Matrix4& OtherMat)
	{
		Init(OtherMat.M);
	}

	void Matrix4::Init(float Value)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				M[i][j] = Value;
			}
		}
	}

	void Matrix4::Init(const float(*arr)[4])
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				M[i][j] = arr[i][j];
			}
		}
	}

	void Matrix4::Identity()
	{
		M[0][0] = 1, M[0][1] = 0, M[0][2] = 0, M[0][3] = 0;
		M[1][0] = 0, M[1][1] = 1, M[1][2] = 0, M[1][3] = 0;
		M[2][0] = 0, M[2][1] = 0, M[2][2] = 1, M[2][3] = 0;
		M[3][0] = 0, M[3][1] = 0, M[3][2] = 0, M[3][3] = 1;
	}

	#pragma region Operators

	Matrix4& Matrix4::operator=(const Matrix4& OtherMat)
	{
		Init(OtherMat.M);
		return *this;
	}

	bool Matrix4::operator==(const Matrix4& OtherMat) const
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (M[i][j] != OtherMat(i, j))
					return false;
			}
		}

		return true;
	}

	bool Matrix4::operator!=(const Matrix4& OtherMat) const
	{
		return !(*this == OtherMat);
	}

	const Matrix4 Matrix4::operator+(const Matrix4& OtherMat) const
	{
		Matrix4 result;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result(i, j) = M[i][j] + OtherMat(i, j);
			}
		}

		return result;
	}

	const Matrix4 Matrix4::operator-(const Matrix4& OtherMat) const
	{
		Matrix4 result;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result(i, j) = M[i][j] - OtherMat(i ,j);
			}
		}

		return result;
	}

	const Matrix4 Matrix4::operator*(const Matrix4& OtherMat) const
	{
		Matrix4 result;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				float val = 0;

				for (int k = 0; k < 4; k++)
				{
					val += M[i][k] * OtherMat(k, j);
				}
				result(i, j) = val;
			}
		}

		return result;
	}

	const Matrix4 Matrix4::operator+(float Value) const
	{
		Matrix4 result;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result(i, j) = M[i][j] + Value;
			}
		}

		return result;
	}

	const Matrix4 Matrix4::operator-(float Value) const
	{
		Matrix4 result;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result(i, j) = M[i][j] - Value;
			}
		}

		return result;
	}

	const Matrix4 Matrix4::operator*(float Value) const
	{
		Matrix4 result;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result(i, j) = M[i][j] * Value;
			}
		}

		return result;
	}

	const Matrix4 Matrix4::operator/(float Value) const
	{
		Matrix4 result;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result(i, j) = (Value == 0) ? FLT_MAX : M[i][j] / Value;
			}
		}

		return result;
	}

	Matrix4& Matrix4::operator+=(const Matrix4& OtherMat)
	{
		*this = *this + OtherMat;
		return *this;
	}

	Matrix4& Matrix4::operator-=(const Matrix4& OtherMat)
	{
		*this = *this - OtherMat;
		return *this;
	}

	Matrix4& Matrix4::operator*=(const Matrix4& OtherMat)
	{
		*this = *this * OtherMat;
		return *this;
	}

	Matrix4& Matrix4::operator+=(float Value)
	{
		*this = *this + Value;
		return *this;
	}

	Matrix4& Matrix4::operator-=(float Value)
	{
		*this = *this - Value;
		return *this;
	}

	Matrix4& Matrix4::operator*=(float Value)
	{
		*this = *this * Value;
		return *this;
	}

	Matrix4& Matrix4::operator/=(float Value)
	{
		*this = *this / Value;
		return *this;
	}

	#pragma endregion

	Matrix4 Matrix4::Transpose() const
	{
		Matrix4 result;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result[j][i] = M[i][j];
			}
		}

		return result;
	}

	const Vector4 Matrix4::operator*(const Vector4& Vec) const
	{
		Vector4 result(
			M[0][0] * Vec.x + M[0][1] * Vec.y + M[0][2] * Vec.z + M[0][3] * Vec.w,
			M[1][0] * Vec.x + M[1][1] * Vec.y + M[1][2] * Vec.z + M[1][3] * Vec.w,
			M[2][0] * Vec.x + M[2][1] * Vec.y + M[2][2] * Vec.z + M[2][3] * Vec.w,
			M[3][0] * Vec.x + M[3][1] * Vec.y + M[3][2] * Vec.z + M[3][3] * Vec.w
		);

		return result;
	}

	const Vector3 Matrix4::operator*(const Vector3& Vec) const
	{
		Vector4 result(Vec, 1.0f);
		result = *this * result;
		return Vector3(result);
	}

	float* const Matrix4::operator[](int index) const
	{
		if (index < 0)
			index = 0;
		else if (index > 3)
			index = 3;

		return (float* const)M[index];
	}

	float Matrix4::Determinant() const
	{
		return (
			  M[0][0] * (
				  M[1][1] * (M[2][2] * M[3][3] - M[3][2] * M[2][3])
				- M[1][2] * (M[2][1] * M[3][3] - M[2][3] * M[3][1])
				+ M[1][3] * (M[2][1] * M[3][2] - M[3][1] * M[2][2])
				)
			- M[0][1] * (
				  M[1][0] * (M[2][2] * M[3][3] - M[3][2] * M[2][3])
				- M[1][2] * (M[2][0] * M[3][3] - M[3][0] * M[2][3])
				+ M[1][3] * (M[2][0] * M[3][2] - M[3][0] * M[2][2])
				)
			+ M[0][2] * (
				  M[1][0] * (M[2][1] * M[3][3] - M[3][1] * M[2][3])
				- M[1][1] * (M[2][0] * M[3][3] - M[3][0] * M[2][3])
				+ M[1][3] * (M[2][0] * M[3][1] - M[3][0] * M[2][1])
				)
			- M[0][3] * (
				  M[1][0] * (M[2][1] * M[3][2] - M[3][1] * M[2][2])
				- M[1][1] * (M[2][0] * M[3][2] - M[3][0] * M[2][2])
				+ M[1][2] * (M[2][0] * M[3][1] - M[3][0] * M[2][1])
				)
			);
	}

	Matrix4 Matrix4::AdjointTranspose() const
	{
		Matrix4 mat;

		/* First row */
		mat(0, 0) =   ( M[1][1] * (M[2][2] * M[3][3] - M[3][2] * M[2][3])
					  - M[1][2] * (M[2][1] * M[3][3] - M[3][1] * M[2][3])
					  + M[1][3] * (M[2][1] * M[3][2] - M[3][1] * M[2][2]));
		
		mat(0, 1) = - ( M[1][0] * (M[2][2] * M[3][3] - M[3][2] * M[2][3])
					  - M[1][2] * (M[2][0] * M[3][3] - M[3][0] * M[2][3])
					  + M[1][3] * (M[2][0] * M[3][2] - M[3][0] * M[2][2]));

		mat(0, 2) =   ( M[1][0] * (M[2][1] * M[3][3] - M[3][1] * M[2][3])
					  - M[1][1] * (M[2][0] * M[3][3] - M[3][0] * M[2][3])
					  + M[1][3] * (M[2][0] * M[3][1] - M[3][0] * M[2][1]));

		mat(0, 3) = - ( M[1][0] * (M[2][1] * M[3][2] - M[3][1] * M[2][2])
					  - M[1][1] * (M[2][0] * M[3][2] - M[3][0] * M[2][2])
					  + M[1][2] * (M[2][0] * M[3][1] - M[3][0] * M[2][1]));

		/* Second Row */
		mat(1, 0) = - ( M[0][1] * (M[2][2] * M[3][3] - M[3][2] * M[2][3])
					  - M[0][2] * (M[2][1] * M[3][3] - M[3][1] * M[2][3])
					  + M[0][3] * (M[2][1] * M[3][2] - M[3][1] * M[2][2]));
		
		mat(1, 1) =   ( M[0][0] * (M[2][2] * M[3][3] - M[3][2] * M[2][3])
					  - M[0][2] * (M[2][0] * M[3][3] - M[3][0] * M[2][3])
					  + M[0][3] * (M[2][0] * M[3][2] - M[3][0] * M[2][2]));

		mat(1, 2) = - ( M[0][0] * (M[2][1] * M[3][3] - M[3][1] * M[2][3])
					  - M[0][1] * (M[2][0] * M[3][3] - M[3][0] * M[2][3])
					  + M[0][3] * (M[2][0] * M[3][1] - M[3][0] * M[2][1]));

		mat(1, 3) =   ( M[0][0] * (M[2][1] * M[3][2] - M[3][1] * M[2][2])
					  - M[0][1] * (M[2][0] * M[3][2] - M[3][0] * M[2][2])
					  + M[0][2] * (M[2][0] * M[3][1] - M[3][0] * M[2][1]));

		/* Third Row */
		mat(2, 0) =   ( M[0][1] * (M[1][2] * M[3][3] - M[3][2] * M[1][3])
					  - M[0][2] * (M[1][1] * M[3][3] - M[3][1] * M[1][3])
					  + M[0][3] * (M[1][1] * M[3][2] - M[3][1] * M[1][2]));
		
		mat(2, 1) = - ( M[0][0] * (M[1][2] * M[3][3] - M[3][2] * M[1][3])
					  - M[0][2] * (M[1][0] * M[3][3] - M[3][0] * M[1][3])
					  + M[0][3] * (M[1][0] * M[3][2] - M[3][0] * M[1][2]));

		mat(2, 2) =   ( M[0][0] * (M[1][1] * M[3][3] - M[3][1] * M[1][3])
					  - M[0][1] * (M[1][0] * M[3][3] - M[3][0] * M[1][3])
					  + M[0][3] * (M[1][0] * M[3][1] - M[3][0] * M[1][1]));

		mat(2, 3) = - ( M[0][0] * (M[1][1] * M[3][2] - M[3][1] * M[1][2])
					  - M[0][1] * (M[1][0] * M[3][2] - M[3][0] * M[1][2])
					  + M[0][2] * (M[1][0] * M[3][1] - M[3][0] * M[1][1]));

		/* Fourth Row */
		mat(3, 0) = - ( M[0][1] * (M[1][2] * M[2][3] - M[2][2] * M[1][3])
					  - M[0][2] * (M[1][1] * M[2][3] - M[2][1] * M[1][3])
					  + M[0][3] * (M[1][1] * M[2][2] - M[2][1] * M[1][2]));
		
		mat(3, 1) =   ( M[0][0] * (M[1][2] * M[2][3] - M[2][2] * M[1][3])
					  - M[0][2] * (M[1][0] * M[2][3] - M[2][0] * M[1][3])
					  + M[0][3] * (M[1][0] * M[2][2] - M[2][0] * M[1][2]));

		mat(3, 2) = - ( M[0][0] * (M[1][1] * M[2][3] - M[2][1] * M[1][3])
					  - M[0][1] * (M[1][0] * M[2][3] - M[2][0] * M[1][3])
					  + M[0][3] * (M[1][0] * M[2][1] - M[2][0] * M[1][1]));

		mat(3, 3) =   ( M[0][0] * (M[1][1] * M[2][2] - M[2][1] * M[1][2])
					  - M[0][1] * (M[1][0] * M[2][2] - M[2][0] * M[1][2])
					  + M[0][2] * (M[1][0] * M[2][1] - M[2][0] * M[1][1]));

		return mat;
	}

	Matrix4 Matrix4::Adjoint() const
	{
		Matrix4 result(AdjointTranspose());

		result = result.Transpose();

		return result;
	}

	Matrix4 Matrix4::Inverse() const
	{
		Matrix4 mat;

		// Get the adjoint of the matrix
		mat = this->Adjoint();

		// Divide the adjoint by determinant
		mat /= this->Determinant();

		return mat;
	}

	Vector3 Matrix4::ExtractTranslation(const Matrix4& Mat)
	{
		return Vector3(Mat(0, 3), Mat(1, 3), Mat(2, 3));
	}

	Vector3 Matrix4::ExtractScale(const Matrix4& Mat)
	{
		GM::Vector3 ScaleVector;
		ScaleVector.x = Utility::Sqrt(Mat(0, 0) * Mat(0, 0) + Mat(1, 0) * Mat(1, 0) + Mat(2, 0) * Mat(2, 0));
		ScaleVector.y = Utility::Sqrt(Mat(0, 1) * Mat(0, 1) + Mat(1, 1) * Mat(1, 1) + Mat(2, 1) * Mat(2, 1));
		ScaleVector.z = Utility::Sqrt(Mat(0, 2) * Mat(0, 2) + Mat(1, 2) * Mat(1, 2) + Mat(2, 2) * Mat(2, 2));

		return ScaleVector;
	}

	/* Non Member functions */
	std::ostream& operator<<(std::ostream& Out, const Matrix4& Mat)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				Out << Mat(i, j) << ' ';

			Out << '\n';
		}

		return Out;
	}
}