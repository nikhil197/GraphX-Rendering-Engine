#include "GMPch.h"
#include "BoundingBox.h"

#include "MathUtility.h"

namespace GM
{
	BoundingBox::BoundingBox(const std::vector<Vector3>& Points)
		: Min(Vector3()),
		Max(Vector3()),
		IsValid(false)
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


}