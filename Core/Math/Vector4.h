// Core/Math/Vector4.h
#pragma once
#include <cmath>

/**
 * @file Vector4.h
 * @brief Defines the Vector4 class for 4D vector or quaternion math operations.
 */

 /**
  * @class Vector4
  * @brief Represents a 4D vector, which can be used for homogeneous coordinates or quaternions.
  */
class Vector4 {
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
     * @brief W component of the vector.
     */
    float w;

    /**
     * @brief Default constructor. Initializes the vector to (0, 0, 0, 0).
     */
    Vector4() : x(0), y(0), z(0), w(0) {}

    /**
     * @brief Constructs a vector with the given x, y, z, w components.
     * @param x X component.
     * @param y Y component.
     * @param z Z component.
     * @param w W component.
     */
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    /**
     * @brief Calculates the Euclidean length (magnitude) of the vector.
     * @return The length of the vector.
     */
    float Length() const 
    {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    /**
     * @brief Normalizes the vector to unit length.
     *        If the vector length is zero, it remains unchanged.
     */
    void Normalize() 
    {
        float len = Length();
        if (len > 0) { x /= len; y /= len; z /= len; w /= len; }
    }

    /**
     * @brief Adds this vector with another vector.
     * @param rhs The vector to add.
     * @return A new vector that is the sum of this vector and rhs.
     */
    Vector4 operator+(const Vector4& rhs) const 
    {
        return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }

    /**
     * @brief Subtracts another vector from this vector.
     * @param rhs The vector to subtract.
     * @return A new vector that is the difference between this vector and rhs.
     */
    Vector4 operator-(const Vector4& rhs) const 
    {
        return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }

    /**
     * @brief Multiplies the vector by a scalar.
     * @param scalar The scalar value.
     * @return A new vector scaled by the given scalar.
     */
    Vector4 operator*(float scalar) const 
    {
        return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    /**
     * @brief Computes the dot product between this vector and another.
     * @param rhs The other vector.
     * @return The dot product (scalar value).
     */
    float Dot(const Vector4& rhs) const 
    {
        return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
    }
};
