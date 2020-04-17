#include "GMPch.h"
#include "BoundingBox.h"

#include "MathUtility.h"
#include "Matrices/Matrix4.h"
#include "Vectors/Vector4.h"

#include "BoxBounds.h"

namespace GM
{
	BoundingBox::BoundingBox(const std::vector<Vector3>& Points)
		: Min(Vector3()),
		Max(Vector3())
	{
		for (const Vector3& Point : Points)
		{
			*this += Point;
		}
	}

	BoundingBox& BoundingBox::operator+=(const Vector3& Point)
	{
		if (IsValid)
		{
			Min.x = Utility::Min(Min.x, Point.x);
			Min.y = Utility::Min(Min.y, Point.y);
			Min.z = Utility::Min(Min.z, Point.z);

			Max.x = Utility::Max(Max.x, Point.x);
			Max.y = Utility::Max(Max.y, Point.y);
			Max.z = Utility::Max(Max.z, Point.z);
		}
		else
		{
			Min = Max = Point;
			IsValid = true;
		}

		return *this;
	}

	BoundingBox BoundingBox::operator+(const Vector3& Point) const
	{
		return BoundingBox(*this) += Point;
	}

	BoundingBox& BoundingBox::operator+=(const BoundingBox& Other)
	{
		if (IsValid && Other.IsValid)
		{
			Min.x = Utility::Min(Min.x, Other.Min.x);
			Min.y = Utility::Min(Min.y, Other.Min.y);
			Min.z = Utility::Min(Min.z, Other.Min.z);

			Max.x = Utility::Max(Max.x, Other.Max.x);
			Max.y = Utility::Max(Max.y, Other.Max.y);
			Max.z = Utility::Max(Max.z, Other.Max.z);
		}
		else if (Other.IsValid)
		{
			*this = Other;
		}

		return *this;
	}

	BoundingBox BoundingBox::operator+(const BoundingBox& OtherBox) const
	{
		return BoundingBox(*this) += OtherBox;
	}

	void BoundingBox::ShiftBy(const Vector3& Delta)
	{
		Min += Delta;
		Max += Delta;
	}

	BoundingBox BoundingBox::ShiftBy(const Vector3& Delta) const
	{
		return BoundingBox(Min + Delta, Max + Delta);
	}

	void BoundingBox::MoveTo(const Vector3& Location)
	{
		Vector3 Offset = Location - GetCenter();
		Min += Offset;
		Max += Offset;
	}

	void BoundingBox::Transform(const class Matrix4& TransformationMat)
	{
		static Vector3 VertexPositions[8];
		VertexPositions[0].x = Min.x, VertexPositions[0].y = Min.y, VertexPositions[0].z = Max.z;
		VertexPositions[1].x = Max.x, VertexPositions[1].y = Min.y, VertexPositions[1].z = Max.z;
		VertexPositions[2].x = Max.x, VertexPositions[2].y = Max.y, VertexPositions[2].z = Max.z;
		VertexPositions[3].x = Min.x, VertexPositions[3].y = Max.y, VertexPositions[3].z = Max.z;

		VertexPositions[4].x = Min.x, VertexPositions[4].y = Min.y, VertexPositions[4].z = Min.z;
		VertexPositions[5].x = Max.x, VertexPositions[5].y = Min.y, VertexPositions[5].z = Min.z;
		VertexPositions[6].x = Max.x, VertexPositions[6].y = Max.y, VertexPositions[6].z = Min.z;
		VertexPositions[7].x = Min.x, VertexPositions[7].y = Max.y, VertexPositions[7].z = Min.z;

		BoundingBox NewBox;
		for (int i = 0; i < 8; i++)
		{
			Vector4 TransformedVector = TransformationMat * Vector4(VertexPositions[i], 1.0f);
			NewBox += Vector3(TransformedVector.x, TransformedVector.y, TransformedVector.z);
		}

		Min = NewBox.Min;
		Max = NewBox.Max;
	}

	void BoundingBox::Transform(const struct BoxBounds& Bounds, const Matrix4& TransformationMat)
	{
		static Vector3 VertexPositions[8];
		Vector3 OriginalMin = Bounds.Origin - Bounds.Extent;
		Vector3 OriginalMax = Bounds.Origin + Bounds.Extent;


		VertexPositions[0].x = OriginalMin.x, VertexPositions[0].y = OriginalMin.y, VertexPositions[0].z = OriginalMax.z;
		VertexPositions[1].x = OriginalMax.x, VertexPositions[1].y = OriginalMin.y, VertexPositions[1].z = OriginalMax.z;
		VertexPositions[2].x = OriginalMax.x, VertexPositions[2].y = OriginalMax.y, VertexPositions[2].z = OriginalMax.z;
		VertexPositions[3].x = OriginalMin.x, VertexPositions[3].y = OriginalMax.y, VertexPositions[3].z = OriginalMax.z;

		VertexPositions[4].x = OriginalMin.x, VertexPositions[4].y = OriginalMin.y, VertexPositions[4].z = OriginalMin.z;
		VertexPositions[5].x = OriginalMax.x, VertexPositions[5].y = OriginalMin.y, VertexPositions[5].z = OriginalMin.z;
		VertexPositions[6].x = OriginalMax.x, VertexPositions[6].y = OriginalMax.y, VertexPositions[6].z = OriginalMin.z;
		VertexPositions[7].x = OriginalMin.x, VertexPositions[7].y = OriginalMax.y, VertexPositions[7].z = OriginalMin.z;

		BoundingBox NewBox;
		for (int i = 0; i < 8; i++)
		{
			Vector4 TransformedVector = TransformationMat * Vector4(VertexPositions[i], 1.0f);
			NewBox += Vector3(TransformedVector.x, TransformedVector.y, TransformedVector.z);
		}

		Min = NewBox.Min;
		Max = NewBox.Max;

	}

	bool BoundingBox::RayIntersectionTest(const BoundingBox& Box, const Vector3& Origin, const Vector3& Direction)
	{
		/**
			Using the equation ax + by + cz - d = 0 for a plane,
			<a, b, c> is the normal vector to the plane
			d is the perpendicular distance from origin (0, 0, 0) to the plane
		*/

		float t;
		Vector3 IntersectionPoint;
		
		// Top & Bottom Face
		float DotProduct = Vector3::DotProduct(Direction, Vector3::YAxis);
		if (DotProduct != 0)
		{
			// Top Face
			t = -(Origin.y - Box.Max.y) / DotProduct;
			IntersectionPoint = Origin + Direction * t;
			if (IntersectionPoint.x >= Box.Min.x && IntersectionPoint.x <= Box.Max.x
				&& IntersectionPoint.z >= Box.Min.z && IntersectionPoint.y <= Box.Max.z)
				return true;

			// Bottom Face
			t = -(Origin.y - Box.Min.y) / DotProduct;
			IntersectionPoint = Origin + Direction * t;
			if (IntersectionPoint.x >= Box.Min.x && IntersectionPoint.x <= Box.Max.x
				&& IntersectionPoint.z >= Box.Min.z && IntersectionPoint.y <= Box.Max.z)
				return true;
		}

		// Left & Right Face
		DotProduct = Vector3::DotProduct(Direction, Vector3::XAxis);
		if (DotProduct != 0)
		{
			// Left Face
			t = -(Origin.x - Box.Min.x) / DotProduct;
			IntersectionPoint = Origin + Direction * t;
			if (IntersectionPoint.z >= Box.Min.z && IntersectionPoint.z <= Box.Max.z
				&& IntersectionPoint.y >= Box.Min.y && IntersectionPoint.y <= Box.Max.y)
				return true;

			// Right Face
			t = -(Origin.x - Box.Max.x) / DotProduct;
			IntersectionPoint = Origin + Direction * t;
			if (IntersectionPoint.z >= Box.Min.z && IntersectionPoint.z <= Box.Max.z
				&& IntersectionPoint.y >= Box.Min.y && IntersectionPoint.y <= Box.Max.y)
				return true;
		}

		// Front & Back Face
		DotProduct = Vector3::DotProduct(Direction, Vector3::ZAxis);
		if (DotProduct != 0)
		{
			// Front Face
			t = -(Origin.z - Box.Max.z) / DotProduct;
			IntersectionPoint = Origin + Direction * t;
			if (IntersectionPoint.x >= Box.Min.x && IntersectionPoint.x <= Box.Max.x
				&& IntersectionPoint.y >= Box.Min.y && IntersectionPoint.y <= Box.Max.y)
				return true;

			// Back Face
			t = -(Origin.z - Box.Min.z) / DotProduct;
			IntersectionPoint = Origin + Direction * t;
			if (IntersectionPoint.x >= Box.Min.x && IntersectionPoint.x <= Box.Max.x
				&& IntersectionPoint.y >= Box.Min.y && IntersectionPoint.y <= Box.Max.y)
				return true;
		}

		return false;
	}
}