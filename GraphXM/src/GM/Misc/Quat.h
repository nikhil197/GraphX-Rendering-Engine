#pragma once

#include "Vectors/Vector3.h"

namespace GM
{
	/**
    * Floating point quaternion that can represent a rotation about an axis in 3-D space.
    * The X, Y, Z, W components also double as the Axis/Angle format.
    *
    * Order matters when composing quaternions: C = A * B will yield a quaternion C that logically
    * first applies B then A to any subsequent transformation (right first, then left).
    * Note that this is the opposite order of FTransform multiplication.
    *
    * Example: LocalToWorld = (LocalToWorld * DeltaRotation) will change rotation in local space by DeltaRotation.
    * Example: LocalToWorld = (DeltaRotation * LocalToWorld) will change rotation in world space by DeltaRotation.
    */
    struct Quat
    {
    public:

        /** The quaternion's X-component. */
        float X;

        /** The quaternion's Y-component. */
        float Y;

        /** The quaternion's Z-component. */
        float Z;

        /** The quaternion's W-component. */
        float W;
        
    public:
        /** Identity Quaternion */
        static const Quat Identity;

    public:
        Quat() = default;

       /**
       * Constructor.
       *
       * @param InX X component of the quaternion
       * @param InY Y component of the quaternion
       * @param InZ Z component of the quaternion
       * @param InW W component of the quaternion
       */
       Quat(float InX, float InY, float InZ, float InW);

       /**
       * Constructor.
       *
       * @param InPitch Rotation about the Right Axis (X-Axis)
       * @param InYaw Rotation about the Up Axis (Y - Axis)
       * @param InRoll Rotation about the Forward Axis (Z - Axis)
       */
       Quat(float InPitch, float InYaw, float InRoll);

       /**
       * Creates and initializes a new quaternion from the a rotation around the given axis.
       *
       * @param Axis assumed to be a normalized vector
       * @param Angle angle to rotate above the given axis
       */
       Quat(const Vector3& Axis, float Angle);

    public:
        /* Arithmetic operators */
        const Quat operator+(const Quat& Q) const;

        Quat& operator+=(const Quat& Q);

        const Quat operator-(const Quat& Q) const;

        Quat& operator-=(const Quat& Q);

        const Quat operator*(const Quat& Q) const;

        Quat& operator*=(const Quat& Q);

        /**
        * Rotate a vector by this quaternion.
        *
        * @param V the vector to be rotated
        * @return vector after rotation
        * @see RotateVector
        */
        Vector3 operator*(const Vector3& V) const;

        /**
         * Get the result of scaling this quaternion.
         *
         * @param Scale The scaling factor.
         * @return The result of scaling.
         */
        const Quat operator*(const float Scale) const;
        
        /**
        * Multiply this quaternion by a scaling factor.
        *
        * @param Scale The scaling factor.
        * @return a reference to this after scaling.
        */
        Quat& operator*=(const float Scale);

        /**
         * Divide this quaternion by scale.
         *
         * @param Scale What to divide by.
         * @return new Quaternion of this after division by scale.
         */
        const Quat operator/(const float Scale) const;
        
        /**
         * Divide this quaternion by scale.
         *
         * @param Scale What to divide by.
         * @return a reference to this after scaling.
         */
        Quat& operator/=(const float Scale);

        /**
        * Checks whether two quaternions are identical.
        * This is an exact comparison per-component
        *
        * @param Q The other quaternion.
        * @return true if two quaternion are identical, otherwise false.
        */
        bool operator==(const Quat& Q) const;

        /**
        * Calculates dot product of two quaternions.
        *
        * @param Q The other quaternions.
        * @return The dot product.
        */
        float operator|(const Quat& Q) const;

    public:
        /* Returns the angle by which this quaternion rotates a vector */
        float GetAngle() const;

        /**
        * Get the axis of rotation of the Quaternion.
        * This is the axis around which rotation occurs to transform the canonical coordinate system to the target orientation.
        * For the identity Quaternion which has no such rotation, FVector(1,0,0) is returned.
        * @warning : assumes normalized quaternions.
        */
        Vector3 GetRotationAxis() const;

        /**
        * Normalize this quaternion if it is large enough.
        * If it is too small, returns an identity quaternion.
        *
        * @param Tolerance Minimum squared length of quaternion for normalization.
        */
        void Normalize();

        /**
        * Get the length of this quaternion.
        *
        * @return The length of this quaternion.
        */
        float Size() const;

        /**
         * Get the length squared of this quaternion.
         *
         * @return The length of this quaternion.
         */
        float SizeSquared() const;

        /* Returns the inverse of this quaternion */
        Quat Inverse() const;

        /**
        * get the axis and angle of rotation of this quaternion
        *
        * @param Axis{out] vector of axis of the quaternion
        * @param Angle{out] angle of the quaternion
        * @warning : assumes normalized quaternions.
        */
        void ToAxisAndAngle(Vector3& Axis, float& Angle) const;

        /**
        * Rotate a vector by this quaternion.
        *
        * @param V the vector to be rotated
        * @return vector after rotation
        */
        Vector3 RotateVector(Vector3 V) const;

        /**
         * Rotate a vector by the inverse of this quaternion.
         *
         * @param V the vector to be rotated
         * @return vector after rotation by the inverse of this quaternion.
         */
        Vector3 UnrotateVector(Vector3 V) const;
    };

    /* Premultiplies the vector with a quaterion */
    const Vector3 operator*(const Vector3& V, const Quat& Q);
}