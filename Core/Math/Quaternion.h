// Core/Math/Quaternion.h
#pragma once
#include <cmath>
#include "Matrix4x4.h"

/**
 * @file Quaternion.h
 * @brief Defines the Quaternion class used for representing 3D rotations.
 */

 /**
  * @class Quaternion
  * @brief Represents a quaternion for encoding rotations in 3D space.
  *        Offers normalization, multiplication, and conversion to a rotation matrix.
  */
class Quaternion {
public:
    /**
     * @brief X component of the quaternion (imaginary i).
     */
    float x;

    /**
     * @brief Y component of the quaternion (imaginary j).
     */
    float y;

    /**
     * @brief Z component of the quaternion (imaginary k).
     */
    float z;

    /**
     * @brief W component of the quaternion (real part).
     */
    float w;

    /**
     * @brief Default constructor. Initializes to identity quaternion (0, 0, 0, 1).
     */
    Quaternion() : x(0), y(0), z(0), w(1) {}

    /**
     * @brief Constructs a quaternion with specified components.
     * @param x X component (i).
     * @param y Y component (j).
     * @param z Z component (k).
     * @param w W component (real part).
     */
    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    /**
     * @brief Normalizes the quaternion to unit length.
     *        If the magnitude is zero, it remains unchanged.
     */
    void Normalize() {
        float len = std::sqrt(x * x + y * y + z * z + w * w);
        if (len > 0) {
            x /= len; y /= len; z /= len; w /= len;
        }
    }

    /**
     * @brief Multiplies this quaternion by another (Hamilton product).
     * @param rhs The right-hand side quaternion.
     * @return The result of the quaternion multiplication.
     */
    Quaternion operator*(const Quaternion& rhs) const {
        return Quaternion(
            w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
            w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z,
            w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x,
            w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z);
    }

    /**
     * @brief Converts a quaternion into a 4x4 rotation matrix.
     * @param q The quaternion to convert.
     * @return A 4x4 rotation matrix representing the same rotation as the quaternion.
     */
    static Matrix4x4 ToMatrix(const Quaternion& q) {
        Matrix4x4 mat;
        float xx = q.x * q.x, yy = q.y * q.y, zz = q.z * q.z;
        float xy = q.x * q.y, xz = q.x * q.z, yz = q.y * q.z;
        float wx = q.w * q.x, wy = q.w * q.y, wz = q.w * q.z;

        mat(0, 0) = 1.0f - 2.0f * (yy + zz);
        mat(0, 1) = 2.0f * (xy + wz);
        mat(0, 2) = 2.0f * (xz - wy);
        mat(0, 3) = 0.0f;

        mat(1, 0) = 2.0f * (xy - wz);
        mat(1, 1) = 1.0f - 2.0f * (xx + zz);
        mat(1, 2) = 2.0f * (yz + wx);
        mat(1, 3) = 0.0f;

        mat(2, 0) = 2.0f * (xz + wy);
        mat(2, 1) = 2.0f * (yz - wx);
        mat(2, 2) = 1.0f - 2.0f * (xx + yy);
        mat(2, 3) = 0.0f;

        mat(3, 0) = 0.0f;
        mat(3, 1) = 0.0f;
        mat(3, 2) = 0.0f;
        mat(3, 3) = 1.0f;

        return mat;
    }
};
