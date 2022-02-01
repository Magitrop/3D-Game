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
	extern const Vector3&& zero;
	extern const Vector3&& one;
	extern const Vector3&& up;
	extern const Vector3&& down;
	extern const Vector3&& right;
	extern const Vector3&& left;
	extern const Vector3&& forward;
	extern const Vector3&& backward;

	extern float Length(Vector2 a);
	extern float Length(Vector3 a);
	extern float Length(Vector4 a);
	extern float Dot(Vector2 a, Vector2 b);
	extern float Dot(Vector3 a, Vector3 b);
	extern float Dot(Vector4 a, Vector4 b);
	extern Vector3 Cross(Vector3 a, Vector3 b);
	extern Vector2 Perpendicular(Vector3 a); // counter-clockwise
	extern float Angle(Vector2 a, Vector2 b);
	extern float Angle(Vector3 a, Vector3 b);
	extern Vector3 MultiplyPoint(const Matrix4x4& m, const Vector3& p);

	extern std::string VectorToString(const Vector2& vec);
	extern std::string VectorToString(const Vector3& vec);
	extern std::string VectorToString(const Vector4& vec);
}

std::ostream& operator << (std::ostream& stream, const Vector2& vec);
std::ostream& operator << (std::ostream& stream, const Vector3& vec);
std::ostream& operator << (std::ostream& stream, const Vector4& vec);

Vector2& operator * (Vector2& vec, int value);
Vector2& operator * (Vector2& vec, float value);
Vector2& operator * (Vector2& vec, double value);
Vector3& operator * (Vector3& vec, int value);
Vector3& operator * (Vector3& vec, float value);
Vector3& operator * (Vector3& vec, double value);
Vector4& operator * (Vector4& vec, int value);
Vector4& operator * (Vector4& vec, float value);
Vector4& operator * (Vector4& vec, double value);