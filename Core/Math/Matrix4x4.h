// Core/Math/Matrix4x4.h
#pragma once
#include <array>
#include <cmath>

/**
 * @file Matrix4x4.h
 * @brief Defines the Matrix4x4 class for 3D transformations and projections.
 */

 /**
  * @class Matrix4x4
  * @brief Represents a 4x4 matrix commonly used for 3D transformations including
  *        translation, scaling, rotation, and projection.
  */
class Matrix4x4 {
public:
    /**
     * @brief The 16 elements of the matrix in row-major order.
     */
    std::array<float, 16> m;

    /**
     * @brief Default constructor. Initializes the matrix as an identity matrix.
     */
    Matrix4x4() {
        for (int i = 0; i < 16; i++)
            m[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }

    /**
     * @brief Accesses or modifies an element in the matrix.
     * @param row The row index (0 to 3).
     * @param col The column index (0 to 3).
     * @return A reference to the element at (row, col).
     */
    float& operator()(int row, int col) { return m[row * 4 + col]; }

    /**
     * @brief Accesses a constant element in the matrix.
     * @param row The row index (0 to 3).
     * @param col The column index (0 to 3).
     * @return The value of the element at (row, col).
     */
    float operator()(int row, int col) const { return m[row * 4 + col]; }

    /**
     * @brief Multiplies this matrix by another 4x4 matrix.
     * @param rhs The right-hand side matrix.
     * @return A new matrix that is the result of the multiplication.
     */
    Matrix4x4 operator*(const Matrix4x4& rhs) const {
        Matrix4x4 result;
        for (int row = 0; row < 4; ++row)
            for (int col = 0; col < 4; ++col) {
                result(row, col) = 0.0f;
                for (int k = 0; k < 4; ++k)
                    result(row, col) += (*this)(row, k) * rhs(k, col);
            }
        return result;
    }

    /**
     * @brief Creates a perspective projection matrix.
     * @param fov Field of view in radians.
     * @param aspect Aspect ratio (width / height).
     * @param near Near clipping plane.
     * @param far Far clipping plane.
     * @return A perspective projection matrix.
     */
    static Matrix4x4 Perspective(float fov, float aspect, float near, float far) {
        Matrix4x4 proj;
        float tanHalfFOV = std::tan(fov / 2.0f);
        proj(0, 0) = 1.0f / (aspect * tanHalfFOV);
        proj(1, 1) = 1.0f / tanHalfFOV;
        proj(2, 2) = far / (far - near);
        proj(2, 3) = 1.0f;
        proj(3, 2) = -(far * near) / (far - near);
        proj(3, 3) = 0.0f;
        return proj;
    }

    /**
     * @brief Creates a translation matrix.
     * @param x Translation along the X axis.
     * @param y Translation along the Y axis.
     * @param z Translation along the Z axis.
     * @return A matrix that applies the specified translation.
     */
    static Matrix4x4 Translation(float x, float y, float z) {
        Matrix4x4 mat;
        mat(3, 0) = x;
        mat(3, 1) = y;
        mat(3, 2) = z;
        return mat;
    }

    /**
     * @brief Creates a scaling matrix.
     * @param x Scale factor along the X axis.
     * @param y Scale factor along the Y axis.
     * @param z Scale factor along the Z axis.
     * @return A matrix that scales by the given factors.
     */
    static Matrix4x4 Scale(float x, float y, float z) {
        Matrix4x4 mat;
        mat(0, 0) = x;
        mat(1, 1) = y;
        mat(2, 2) = z;
        return mat;
    }

    /**
     * @brief Creates a rotation matrix around the X axis.
     * @param angle Rotation angle in radians.
     * @return A matrix that rotates around the X axis.
     */
    static Matrix4x4 RotationX(float angle) {
        Matrix4x4 mat;
        float c = std::cos(angle), s = std::sin(angle);
        mat(1, 1) = c; mat(1, 2) = s;
        mat(2, 1) = -s; mat(2, 2) = c;
        return mat;
    }

    /**
     * @brief Creates a rotation matrix around the Y axis.
     * @param angle Rotation angle in radians.
     * @return A matrix that rotates around the Y axis.
     */
    static Matrix4x4 RotationY(float angle) {
        Matrix4x4 mat;
        float c = std::cos(angle), s = std::sin(angle);
        mat(0, 0) = c; mat(0, 2) = -s;
        mat(2, 0) = s; mat(2, 2) = c;
        return mat;
    }

    /**
     * @brief Creates a rotation matrix around the Z axis.
     * @param angle Rotation angle in radians.
     * @return A matrix that rotates around the Z axis.
     */
    static Matrix4x4 RotationZ(float angle) {
        Matrix4x4 mat;
        float c = std::cos(angle), s = std::sin(angle);
        mat(0, 0) = c; mat(0, 1) = s;
        mat(1, 0) = -s; mat(1, 1) = c;
        return mat;
    }
};
