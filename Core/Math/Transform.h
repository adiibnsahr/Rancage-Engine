// Core/Math/Transform.h
#pragma once
#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix4x4.h"

/**
 * @file Transform.h
 * @brief Defines the Transform class used to represent an object's position, rotation, and scale in 3D space.
 */

 /**
  * @class Transform
  * @brief Represents the transformation of an object in 3D space,
  *        including position, rotation (quaternion), and scale.
  */
class Transform {
public:
    /**
     * @brief Position of the object in world or local space.
     */
    Vector3 position;

    /**
     * @brief Rotation of the object represented as a quaternion.
     */
    Quaternion rotation;

    /**
     * @brief Scale of the object along each axis.
     */
    Vector3 scale;

    /**
     * @brief Default constructor. Initializes position to (0,0,0),
     *        rotation to identity, and scale to (1,1,1).
     */
    Transform() : position(0, 0, 0), rotation(), scale(1, 1, 1) {}

    /**
     * @brief Generates the 4x4 transformation matrix representing the object's
     *        local transformation (scale * rotation * translation).
     * @return A 4x4 matrix combining scale, rotation, and translation.
     */
    Matrix4x4 GetMatrix() const {
        Matrix4x4 T = Matrix4x4::Translation(position.x, position.y, position.z);
        Matrix4x4 R = Quaternion::ToMatrix(rotation);
        Matrix4x4 S = Matrix4x4::Scale(scale.x, scale.y, scale.z);
        return S * R * T;
    }
};
