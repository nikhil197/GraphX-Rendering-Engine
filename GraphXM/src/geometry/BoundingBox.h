#pragma once

#include "vectors\Vector3.h"

namespace GraphXMaths
{
	/*
	*	Represents an Axis Aligned Bounding box
	*
	*	Describes an Axis aligned extent in 3 dimensions
	*/
	struct BoundingBox
	{
	public:
		/* Stores the minimun extents of the bounding box */
		Vector3 Min;

		/* Stores the maximum extents of the bounding box */
		Vector3 Max;

		/* Flag whether the box is valid or not */
		bool IsValid;

	public:
		/* Empty Default constructor */
		BoundingBox() = default;

		/* Construct from a given range */
		BoundingBox(const Vector3& InMin, const Vector3& InMax)
			: Min(InMin), Max(InMax), IsValid(true)
		{}

		/* Construct a bounding box from the points */
		BoundingBox(const std::vector<Vector3>& Points);

	public:
		inline bool operator==(const BoundingBox& Other) const
		{
			return Min == Other.Min && Max == Other.Max;
		}

		/* Adds a new point to the box */
		BoundingBox& operator+=(const Vector3& Point);

		/* Returns a new box obtained after adding point to this box */
		BoundingBox operator+(const Vector3& Point) const;

		/* Adds a box to the box */
		BoundingBox& operator+=(const BoundingBox& OtherBox);

		/* Returns a new box obtained after adding another box to this box */
		BoundingBox operator+(const BoundingBox& OtherBox) const;
	};
}