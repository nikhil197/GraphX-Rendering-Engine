#pragma once

#include "Vectors/Vector3.h"

namespace GM
{
	/*
	*	Represents an Axis Aligned Bounding box
	*
	*	Describes an Axis aligned extent in 3 dimensions
	*/
	struct BoundingBox
	{
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

	public:
		/* Returns whether the point is inside the box */
		inline bool IsInside(const Vector3& Point) const
		{
			return (Point.x > Min.x && Point.x < Max.x)
				&& (Point.y > Min.y && Point.y < Max.y)
				&& (Point.z > Min.z && Point.z < Max.z);
		}

		/* Returns whether the point is inside or on the box */
		inline bool IsInsideOrOn(const Vector3& Point) const
		{
			return (Point.x >= Min.x && Point.x <= Max.x)
				&& (Point.y >= Min.y && Point.y <= Max.y)
				&& (Point.z >= Min.z && Point.z <= Max.z);
		}

		/* Returns the center of the Bounding Box */
		inline Vector3 GetCenter() const
		{
			return (Min + Max) * 0.5f;
		}

		/* Returns extents of the box (how much it extends on both sides of the center in all 3 directions) */
		inline Vector3 GetExtent() const
		{
			return 0.5f * (Max - Min);
		}

		/* Gets the center and extent of the box */
		inline void GetCenterAndExtent(/*out*/Vector3& Center, /*out*/Vector3& Extent) const
		{
			Extent = GetExtent();
			Center = Min + Extent;
		}

		/* Returns the volume of the box */
		inline float GetVolume() const
		{
			return (Max.x - Min.x) * (Max.y - Min.y) * (Max.z - Max.z);
		}

		/* Move this bounding box by amount Delta */
		void ShiftBy(const Vector3& Delta);

		/* Returns a bounding box resulting from the movement of this box by Delta */
		BoundingBox ShiftBy(const Vector3& Delta) const;

		/* Moves the box's center to the Location */
		void MoveTo(const Vector3& Location);

		/* Transforms the bounding box using given transformation matrix */
		void Transform(const class Matrix4& TransformationMat);

	public:
		/* Stores the minimun extents of the bounding box */
		Vector3 Min;

		/* Stores the maximum extents of the bounding box */
		Vector3 Max;

		/* Flag whether the box is valid or not */
		bool IsValid;

	};
}