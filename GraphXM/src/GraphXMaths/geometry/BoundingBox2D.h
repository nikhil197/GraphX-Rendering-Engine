#pragma once

#include "Vectors/Vector2.h"

namespace GraphXMaths
{
	/*
	*	Represents an Axis Aligned Bounding box
	*
	*	Describes an Axis aligned extent in 3 dimensions
	*/
	struct BoundingBox2D
	{
	public:
		/* Stores the minimun extents of the bounding box */
		Vector2 Min;

		/* Stores the maximum extents of the bounding box */
		Vector2 Max;

		/* Flag whether the box is valid or not */
		bool IsValid;

	public:
		/* Empty Default constructor */
		BoundingBox2D() = default;

		/* Construct from a given range */
		BoundingBox2D(const Vector2& InMin, const Vector2& InMax)
			: Min(InMin), Max(InMax), IsValid(true)
		{}

		/* Construct a bounding box from the points */
		BoundingBox2D(const std::vector<Vector2>& Points);

	public:
		inline bool operator==(const BoundingBox2D& Other) const
		{
			return Min == Other.Min && Max == Other.Max;
		}

		/* Adds a new point to the box */
		BoundingBox2D& operator+=(const Vector2& Point);

		/* Returns a new box obtained after adding point to this box */
		BoundingBox2D operator+(const Vector2& Point) const;

		/* Adds a box to the box */
		BoundingBox2D& operator+=(const BoundingBox2D& OtherBox);

		/* Returns a new box obtained after adding another box to this box */
		BoundingBox2D operator+(const BoundingBox2D& OtherBox) const;
	};
}