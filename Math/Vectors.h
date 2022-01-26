#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using glm::Vector2;
using glm::Vector3;
using glm::Vector4;

namespace Vectors
{
#pragma region Length

	static float Length(Vector2 a);
	static float Length(Vector3 a);
	static float Length(Vector4 a);

#pragma endregion

#pragma region Dot

	static float Dot(Vector2 a, Vector2 b);
	static float Dot(Vector3 a, Vector3 b);
	static float Dot(Vector4 a, Vector4 b);

#pragma endregion


#pragma region Cross

	static Vector3&& Cross(Vector3 a, Vector3 b);

#pragma endregion

#pragma region Perpendicular

	// counter-clockwise
	static Vector2&& Perpendicular(Vector3 a);

#pragma endregion

#pragma region Angle

	static float Angle(Vector2 a, Vector2 b);
	static float Angle(Vector3 a, Vector3 b);

#pragma endregion
}