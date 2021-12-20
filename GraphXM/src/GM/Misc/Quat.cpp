#include "GMPch.h"
#include "Quat.h"

#include "MathUtility.h"
#include "Matrices/Matrix4.h"
#include "Rotator.h"

namespace GM
{
    void QuaternionMultiply(Quat& Result, const Quat& Q1, const Quat& Q2)
    {
        Vector3 Axis1(Q1.X, Q1.Y, Q1.Z);
        Vector3 Axis2(Q2.X, Q2.Y, Q2.Z);

        Vector3 ResultAxis = Vector3::CrossProduct(Axis1, Axis2);

        Result.X = (Q1.W * Axis2.x) + (Q2.W * Axis1.x) + ResultAxis.x;
        Result.Y = (Q1.W * Axis2.y) + (Q2.W * Axis1.y) + ResultAxis.y;
        Result.Z = (Q1.W * Axis2.z) + (Q2.W * Axis1.z) + ResultAxis.z;
        Result.W = (Q1.W * Q2.W) - Vector3::DotProduct(Axis1, Axis2);
    }

    const Quat Quat::Identity(0, 0, 0, 1);

    Quat::Quat(float InX, float InY, float InZ, float InW)
        : X(InX), Y(InY), Z(InZ), W(InW)
    { }

    Quat::Quat(float InPitch, float InYaw, float InRoll)
    {
        // cos and sin for roll, pitch and yaw respectively
        float SR, SP, SY;
        float CR, CP, CY;

        Utility::SinAndCos(SP, CP, InPitch * 0.5f);
        Utility::SinAndCos(SY, CY, InYaw * 0.5f);
        Utility::SinAndCos(SR, CR, InRoll * 0.5f);

        X = SP * CY * CR - CP * SY * SR;
        Y = CP * SY * CR + SP * CY * SR;
        Z = CP * CY * SR - SP * SY * CR;
        W = CP * CY * CR + SP * SY * SR;
    }

    Quat::Quat(const GM::Rotator& Rotation)
        : Quat(Rotation.Pitch, Rotation.Yaw, Rotation.Roll)
    {
    }

    Quat::Quat(const Vector3& Axis, float Angle)
    {
        const float HalfAngle = 0.5f * Angle;
        float Sin = Utility::Sin(HalfAngle);
        float Cos = Utility::Cos(HalfAngle);

        X = Sin * Axis.x;
        Y = Sin * Axis.y;
        Z = Sin * Axis.z;
        W = Cos;
    }

    const Quat Quat::operator+(const Quat& Q) const
    {
        return Quat(X + Q.X, Y + Q.Y, Z + Q.Z, W + Q.W);
    }

    Quat& Quat::operator+=(const Quat& Q)
    {
        X += Q.X;
        Y += Q.Y;
        Z += Q.Z;
        W += Q.W;
        return *this;
    }

    const Quat Quat::operator-(const Quat& Q) const
    {
        return Quat(X - Q.X, Y - Q.Y, Z - Q.Z, W - Q.W);
    }

    Quat& Quat::operator-=(const Quat& Q)
    {
        X -= Q.X;
        Y -= Q.Y;
        Z -= Q.Z;
        W -= Q.W;
        return *this;
    }

    const Quat Quat::operator*(const Quat& Q) const
    {
        Quat Result;
        QuaternionMultiply(Result, *this, Q);
        return Result;
    }

    Quat& Quat::operator*=(const Quat& Q)
    {
        Quat Result;
        QuaternionMultiply(Result, *this, Q);

        this->X = Result.X;
        this->Y = Result.Y;
        this->Z = Result.Z;
        this->W = Result.W;

        return *this;
    }
    
    Vector3 Quat::operator*(const Vector3& V) const
    {
        return RotateVector(V);
    }

    const Quat Quat::operator*(const float Scale) const
    {
        return Quat(X * Scale, Y * Scale, Z * Scale, W * Scale);
    }

    Quat& Quat::operator*=(const float Scale)
    {
        X *= Scale;
        Y *= Scale;
        Z *= Scale;
        W *= Scale;

        return *this;
    }

    const Quat Quat::operator/(const float Scale) const
    {
        const float Reciprocal = 1.0f / Scale;
        return Quat(X * Reciprocal, Y * Reciprocal, Z * Reciprocal, W * Reciprocal);
    }

    Quat& Quat::operator/=(const float Scale)
    {
        const float Reciprocal = 1.0f / Scale;
        X *= Reciprocal;
        Y *= Reciprocal;
        Z *= Reciprocal;
        W *= Reciprocal;

        return *this;
    }

    bool Quat::operator==(const Quat& Q) const
    {
        return (X == Q.X && Y == Q.Y && Z == Q.Z && W == Q.W);
    }

    float Quat::operator|(const Quat& Q) const
    {
        return (X * Q.X + Y * Q.Y + Z * Q.Z + W * Q.W);
    }

    float Quat::GetAngle() const
    {
        return 2.0f * Utility::ACos(W);
    }

    Vector3 Quat::GetRotationAxis() const
    {
        // To ensure unit quaternion
        const float S = Utility::Sqrt(Utility::Max(1.0f - (W * W), 0.0f));
        if (S > 0.0f)
        {
            return Vector3(X / S, Y / S, Z / S);
        }
        else
        {
            return Vector3::XAxis;
        }
    }

    void Quat::Normalize()
    {
        const float SquareSize = SizeSquared();
        if (SquareSize > 0.0f)
        {
            const float Scale = 1 / SquareSize;

            X *= Scale;
            Y *= Scale;
            Z *= Scale;
            W *= Scale;
        }
        else
        {
            *this = Quat::Identity;
        }
    }

    float Quat::Size() const
    {
        return Utility::Sqrt(X * X + Y * Y + Z * Z + W * W);
    }

    float Quat::SizeSquared() const
    {
        return (X * X + Y * Y + Z * Z + W * W);
    }

    Quat Quat::Inverse() const
    {
        const float scale = 1 / SizeSquared();

        return Quat(-X * scale, -Y * scale, -Z * scale, W * scale);
    }

    void Quat::ToAxisAndAngle(Vector3& Axis, float& Angle) const
    {
        Axis = GetRotationAxis();
        Angle = GetAngle();
    }

    Vector3 Quat::RotateVector(Vector3 V) const
    {
        const Vector3 Axis(X, Y, Z);
        const Vector3 Cross = 2 * Vector3::CrossProduct(Axis, V);
        const Vector3 Result = V + (W * Cross) + Vector3::CrossProduct(Axis, Cross);
        return Result;
    }

    Vector3 Quat::UnrotateVector(Vector3 V) const
    {
        const Vector3 Axis(-X, -Y, -Z);     // Inverse
        const Vector3 Cross = 2 * Vector3::CrossProduct(Axis, V);
        const Vector3 Result = V + (W * Cross) + Vector3::CrossProduct(Axis, Cross);
        return Result;
    }

    Matrix4 Quat::ToMatrix() const
    {
        Matrix4 Result;
        const float xx2 = 2 * X * X;
        const float yy2 = 2 * Y * Y;
        const float zz2 = 2 * Z * Z;

        Result(0, 0) = 1 - yy2 - zz2;
        Result(0, 1) = 2 * (X * Y - W * Z);
        Result(0, 2) = 2 * (X * Z + W * Y);
        Result(0, 3) = 0.0f;

        Result(1, 0) = 2 * (X * Y + W * Z);
        Result(1, 1) = 1 - xx2 - zz2;
        Result(1, 2) = 2 * (Y * Z - W * X);
        Result(1, 3) = 0.0f;

        Result(2, 0) = 2 * (X * Z - W * Y);
        Result(2, 1) = 2 * (Y * Z + W * X);
        Result(2, 2) = 1 - xx2 - yy2;
        Result(2, 3) = 0.0f;
 
        Result(3, 0) = 0.0f;
        Result(3, 1) = 0.0f;
        Result(3, 2) = 0.0f;
        Result(3, 3) = 1.0f;

        return Result;
    }

    const Vector3 operator*(const Vector3& V, const Quat& Q)
    {
        return Q.UnrotateVector(V);
    }
}