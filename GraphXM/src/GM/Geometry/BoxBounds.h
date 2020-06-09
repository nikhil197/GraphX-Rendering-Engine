#pragma once

#include "Vectors/Vector3.h"

namespace GM
{
	/** 
	* Stores the bounds of a box 
	* Used to store the original bounds of a bounding box
	*/
	struct BoxBounds
	{
	public:
		BoxBounds() = default;

		/**
		* Creates and initializes a new instance from the specified parameters.
		*
		* @param InOrigin origin of the bounding box and sphere.
		* @param InBoxExtent half size of box.
		*/
		BoxBounds(const Vector3& InOrigin, const Vector3& InExtents);

		/**
		* Creates and initializes a new instance from the given Box.
		*
		* @param Box The bounding box.
		* @param Sphere The bounding sphere.
		*/
		BoxBounds(const struct BoundingBox& Box);

		/**
		* Creates and initializes a new instance from the given set of points.
		*
		* @param Points The points to be considered for the bounding box.
		*/
		BoxBounds(const std::vector<Vector3>& Points);

	public:
		/* Origin of the bounding box */
		Vector3 Origin;

		/* Extent of the bounding box */
		Vector3 Extent;

		/* Whether this actually stores any bounds or not */
		bool Valid = false;
	};
}