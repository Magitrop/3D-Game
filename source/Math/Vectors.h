#pragma once
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm\ext\matrix_float4x4.hpp>
#include <ostream>

using glm::Vector2;
using glm::Vector3;
using glm::Vector4;
using glm::Matrix4x4;

namespace Vectors
{
	static const Vector3&& zero = Vector3(0, 0, 0);
	static const Vector3&& one = Vector3(1, 1, 1);
	static const Vector3&& up = Vector3(0, 1, 0);
	static const Vector3&& down = Vector3(0, -1, 0);
	static const Vector3&& right = Vector3(1, 0, 0);
	static const Vector3&& left = Vector3(-1, 0, 0);
	static const Vector3&& forward = Vector3(0, 0, 1);
	static const Vector3&& backward = Vector3(0, 0, -1);

	static float Length(Vector2 a);
	static float Length(Vector3 a);
	static float Length(Vector4 a);
	static float Dot(Vector2 a, Vector2 b);
	static float Dot(Vector3 a, Vector3 b);
	static float Dot(Vector4 a, Vector4 b);
	static Vector3 Cross(Vector3 a, Vector3 b);
	static Vector2 Perpendicular(Vector3 a); // counter-clockwise
	static float Angle(Vector2 a, Vector2 b);
	static float Angle(Vector3 a, Vector3 b);
	static Vector3 MultiplyPoint(const Matrix4x4& m, const Vector3& p) 
	{
		Vector3 res;
		res.x = (m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z) + m[0][3];
		res.y = (m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z) + m[1][3];
		res.z = (m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z) + m[2][3];
		return res;
	}
}

std::ostream& operator << (std::ostream& stream, const Vector2& vec);
std::ostream& operator << (std::ostream& stream, const Vector3& vec);
std::ostream& operator << (std::ostream& stream, const Vector4& vec);