#include "GMPch.h"
#include "BoundingBox2D.h"

#include "MathUtility.h"

namespace GM
{
	BoundingBox2D::BoundingBox2D(const std::vector<Vector2>& Points)
		: Min(Vector2()),
		Max(Vector2()),
		IsValid(false)
	{
		for (const Vector2& Point : Points)
		{
			*this += Point;
		}
	}

	BoundingBox2D& BoundingBox2D::operator+=(const Vector2& Point)
	{
		if (IsValid)
		{
			Min.x = Utility::Min(Min.x, Point.x);
			Min.y = Utility::Min(Min.y, Point.y);

			Max.x = Utility::Max(Max.x, Point.x);
			Max.y = Utility::Max(Max.y, Point.y);
		}
		else
		{
			Min = Max = Point;
			IsValid = true;
		}

		return *this;
	}

	BoundingBox2D BoundingBox2D::operator+(const Vector2& Point) const
	{
		return BoundingBox2D(*this) += Point;
	}

	BoundingBox2D& BoundingBox2D::operator+=(const BoundingBox2D& Other)
	{
		if (IsValid && Other.IsValid)
		{
			Min.x = Utility::Min(Min.x, Other.Min.x);
			Min.y = Utility::Min(Min.y, Other.Min.y);

			Max.x = Utility::Max(Max.x, Other.Max.x);
			Max.y = Utility::Max(Max.y, Other.Max.y);
		}
		else if (Other.IsValid)
		{
			*this = Other;
		}

		return *this;
	}

	BoundingBox2D BoundingBox2D::operator+(const BoundingBox2D& OtherBox) const
	{
		return BoundingBox2D(*this) += OtherBox;
	}
}