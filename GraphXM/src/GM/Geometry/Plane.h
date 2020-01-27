#pragma once

namespace GM
{
	struct Vector3;

	/* 
	 *	Represents a 3D plane with equation stored in the form xX + yY + zZ = w
	 */
	struct Plane
	{
	public:
		/* Constructors */
		Plane() = default;

		/*
		* Normal is the vector representing the direction perpendicular to the plane 
		*/
		Plane(const Vector3& Normal, float InW);

		/*
		* Normal is the vector representing the direction perpendicular to the plane
		* BasePoint is the point that lies on the plane
		*/
		Plane(const Vector3& Normal, const Vector3& BasePoint);

		/*
		* P1, P2 & P3 are the points lying on the plane
		*/
		Plane(const Vector3& P1, const Vector3& P2, const Vector3& P3);

		inline bool operator==(const Plane& Other)
		{
			return (x == Other.x) && (y == Other.y) && (z == Other.z) && (w == Other.w);
		}

		inline bool operator!=(const Plane& Other)
		{
			return (x != Other.x) || (y != Other.y) || (z != Other.z) || (w == Other.w);
		}

		/**
		* Converts the normal vector into a unit vector
		*/
		void Normalize();

		/**
		* Calculates distance between plane and a point.
		*
		* @param Point The other point.
		* @return >0: point is in front of the plane, <0: behind, =0: on the plane.
		*/
		inline float DistanceFrom(const Vector3& Point);

		/**
		* Calculates distance between two || planes.
		* Doesn't work for planes which are not parellel
		*
		* @param Other The other plane.
		* @return >0: plane is in front of the plane, <0: behind, =0: on the plane.
		*/
		inline float DistanceFrom(const Plane& Other);

	public:
		/* X - Coefficient of the plane */
		float x;

		/* Y - Coefficient of the plane */
		float y;

		/* Z - Coefficient of the plane */
		float z;

		/* W - Component of the plane */
		float w;
	};
}