// Core/Math/Vector3.h
#pragma once
#include <cmath>

/**
 * @file Vector3.h
 * @brief Defines the Vector3 class for 3D vector math operations.
 */

 /**
  * @class Vector3
  * @brief Represents a 3D vector with x, y, and z components, and provides common vector operations.
  */
class Vector3
{
public:
    /**
     * @brief X component of the vector.
     */
    float x;

    /**
     * @brief Y component of the vector.
     */
    float y;

    /**
     * @brief Z component of the vector.
     */
    float z;

    /**
     * @brief Default constructor. Initializes the vector to (0, 0, 0).
     */
    Vector3() : x(0), y(0), z(0) {}

    /**
     * @brief Constructs a vector with the given x, y, z components.
     * @param x X component.
     * @param y Y component.
     * @param z Z component.
     */
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    /**
     * @brief Calculates the Euclidean length (magnitude) of the vector.
     * @return The length of the vector.
     */
    float Length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    /**
     * @brief Normalizes the vector to unit length.
     *        If the vector length is zero, it remains unchanged.
     */
    void Normalize()
    {
        float len = Length();
        if (len > 0) { x /= len; y /= len; z /= len; }
    }

    /**
     * @brief Adds this vector with another vector.
     * @param rhs The vector to add.
     * @return A new vector that is the sum of this vector and rhs.
     */
    Vector3 operator+(const Vector3& rhs) const 
    {
        return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    /**
     * @brief Subtracts another vector from this vector.
     * @param rhs The vector to subtract.
     * @return A new vector that is the difference between this vector and rhs.
     */
    Vector3 operator-(const Vector3& rhs) const 
    {
        return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    /**
     * @brief Multiplies the vector by a scalar.
     * @param scalar The scalar value.
     * @return A new vector scaled by the given scalar.
     */
    Vector3 operator*(float scalar) const 
    {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    /**
     * @brief Computes the dot product between this vector and another.
     * @param rhs The other vector.
     * @return The dot product (scalar value).
     */
    float Dot(const Vector3& rhs) const 
    {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    /**
     * @brief Computes the cross product between this vector and another.
     * @param rhs The other vector.
     * @return A new vector that is the cross product of this vector and rhs.
     */
    Vector3 Cross(const Vector3& rhs) const
    {
        return Vector3(
            y * rhs.z - z * rhs.y,
            z * rhs.x - x * rhs.z,
            x * rhs.y - y * rhs.x
        );
    }
};
