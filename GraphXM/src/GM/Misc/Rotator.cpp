#include "GMPch.h"
#include "Rotator.h"

namespace GM
{
	const Rotator Rotator::ZeroRotator(0.0f, 0.0f, 0.0f);

	Rotator::Rotator()
		: Pitch(0.0f), Yaw(0.0f), Roll(0.0f)
	{}

	Rotator::Rotator(float InVal)
		: Pitch(InVal), Yaw(InVal), Roll(InVal)
	{}

	Rotator::Rotator(float InPitch, float InYaw, float InRoll)
		: Pitch(InPitch), Yaw(InYaw), Roll(InRoll)
	{}

	const Rotator Rotator::operator+(const Rotator& R) const
	{
		return Rotator(Pitch + R.Pitch, Yaw + R.Yaw, Roll + R.Roll);
	}

	const Rotator Rotator::operator-(const Rotator& R) const
	{
		return Rotator(Pitch - R.Pitch, Yaw - R.Yaw, Roll - R.Roll);
	}

	const Rotator Rotator::operator*(float InScale) const
	{
		return Rotator(Pitch * InScale, Yaw * InScale, Roll * InScale);
	}

	Rotator& Rotator::operator+=(const Rotator& R)
	{
		Pitch += R.Pitch;
		Yaw += R.Yaw;
		Roll += R.Roll;

		return *this;
	}

	Rotator& Rotator::operator-=(const Rotator& R)
	{
		Pitch -= R.Pitch;
		Yaw -= R.Yaw;
		Roll -= R.Roll;

		return *this;
	}

	Rotator& Rotator::operator*=(float InScale)
	{
		Pitch *= InScale;
		Yaw -= InScale;
		Roll -= InScale;

		return *this;
	}

	bool Rotator::operator==(const Rotator& R) const
	{
		return (Pitch == R.Pitch && Yaw == R.Yaw && Roll == R.Roll);
	}

	bool Rotator::operator!=(const Rotator& R) const
	{
		return !operator==(R);
	}

	Rotator Rotator::Add(float InPitch, float InYaw, float InRoll) const
	{
		return Rotator(Pitch + InPitch, Yaw + InYaw, Roll + InRoll);
	}

	Vector3 Rotator::Euler() const
	{
		return Vector3(Pitch, Yaw, Roll);
	}

	Rotator Rotator::MakeFromEuler(const Vector3& Angles)
	{
		return Rotator(Angles.x, Angles.y, Angles.z);
	}
}