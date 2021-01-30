#pragma once
#pragma once

#include "Vectors/Vector3.h"

namespace GM
{
	/*
	* A Container to store the rotation in Tait-Bryan Notation (Yaw, Pitch and Roll)
	*
	* All Rotations are stored in angles
	*/
	struct Rotator
	{
	public:
		/** Rotation around the right axis (around Y axis), Looking up and down (0=Straight Ahead, +Up, -Down) */
		float Pitch;

		/** Rotation around the up axis (around Z axis), Running in circles 0=East, +North, -South. */
		float Yaw;

		/** Rotation around the forward axis (around X axis), Tilting your head, 0=Straight, +Clockwise, -CCW. */
		float Roll;

	public:
		/* A rotator of zero rotation */
		static const Rotator ZeroRotator;

	public:
		Rotator();

		explicit Rotator(float InVal);

		/*
		* Creates a Rotator with the provided pitch, yaw and roll
		* @param Pitch Rotation around right axis
		* @param Yaw Rotation around up axis
		* @param Roll Rotation around forward axis
		*/
		Rotator(float InPitch, float InYaw, float InRoll);

	public:
		/* Operators */
		const Rotator operator+(const Rotator& R) const;

		const Rotator operator-(const Rotator& R) const;

		const Rotator operator*(float InScale) const;

		Rotator& operator+=(const Rotator& R);

		Rotator& operator-=(const Rotator& R);

		Rotator& operator*=(float InScale);

		bool operator==(const Rotator& R) const;

		bool operator!=(const Rotator& R) const;

	public:
		/* Adds to each component of the rotator */
		Rotator Add(float InPitch, float InYaw, float InRoll) const;

		/* Returns the rotation in proper (classic) euler angle notation */
		Vector3 Euler() const;

	public:
		static Rotator MakeFromEuler(const Vector3& Angles);
	};
}