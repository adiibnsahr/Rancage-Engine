// Core/Math/Vector2.h

#pragma once
#include <cmath>

/**
 * @class Vector2
 * @brief Represents a 2D vector with x and y components and operations.
 */
class Vector2
{
public:
	float x, y;

	/**
	 * @brief Default constructor. Initializes the vector to (0, 0).
	 */
	Vector2() : x(0), y(0) {}

	/**
	 * @brief Parameterized constructor.
	 * @param x The x-component.
	 * @param y The y-component.
	 */
	Vector2(float x, float y) : x(x), y(y) {}

	/**
	 * @brief Calculates the length (magnitude) of the vector.
	 * @return The Euclidean length of the vector.
	 */
	float Length() const
	{
		return std::sqrt(x * x + y * y);
	}

	/**
	 * @brief Normalizes the vector to have a length of 1.
	 *        If the vector is zero-length, the operation has no effect.
	 */
	void Normalize()
	{
		float len = Length();
		if (len > 0)
		{
			x /= len;
			y /= len;
		}
	}

	/**
	 * @brief Adds this vector to another vector.
	 * @param rhs The vector to add.
	 * @return A new vector that is the sum of this and rhs.
	 */
	Vector2 operator+(const Vector2& rhs) const 
	{ 
		return Vector2(x + rhs.x, y + rhs.y); 
	}

	/**
	 * @brief Subtracts another vector from this vector.
	 * @param rhs The vector to subtract.
	 * @return A new vector that is the difference of this and rhs.
	 */
	Vector2 operator-(const Vector2& rhs) const
	{
		return Vector2(x - rhs.x, y - rhs.y);
	}

	/**
	 * @brief Multiplies the vector by a scalar.
	 * @param scalar The scalar value to multiply by.
	 * @return A new scaled vector.
	 */
	Vector2 operator*(float scalar) const
	{
		return Vector2(x * scalar, y * scalar);
	}

	/**
	 * @brief Calculates the dot product of this vector and another.
	 * @param rhs The other vector.
	 * @return The dot product as a float.
	 */
	float Dot(const Vector2& rhs) const
	{
		return x * rhs.x + y * rhs.y;
	}
};