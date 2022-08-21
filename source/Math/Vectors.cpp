#include "Vectors.h"
#include <string>
#include <iomanip>

const Vector3&& Vectors::zero = Vector3(0, 0, 0);
const Vector3&& Vectors::one = Vector3(1, 1, 1);
const Vector3&& Vectors::up = Vector3(0, 1, 0);
const Vector3&& Vectors::down = Vector3(0, -1, 0);
const Vector3&& Vectors::right = Vector3(1, 0, 0);
const Vector3&& Vectors::left = Vector3(-1, 0, 0);
const Vector3&& Vectors::forward = Vector3(0, 0, 1);
const Vector3&& Vectors::backward = Vector3(0, 0, -1);

float Vectors::Length(Vector2 a)
{
	return sqrt(a[0] * a[0] + a[1] * a[1]);
}
float Vectors::Length(Vector3 a)
{
	return sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
}
float Vectors::Length(Vector4 a)
{
	return sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2] + a[3] * a[3]);
}

float Vectors::Dot(Vector2 a, Vector2 b)
{
	return a[0] * b[0] + a[1] * b[1];
}
float Vectors::Dot(Vector3 a, Vector3 b)
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
float Vectors::Dot(Vector4 a, Vector4 b)
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

Vector3 Vectors::Cross(Vector3 a, Vector3 b)
{
	float x, y, z;
	x = a.y * b.z - b.y * a.z;
	y = -(a.x * b.z - b.x * a.z);
	z = a.x * b.y - b.x * a.y;
	return Vector3(x, y, z);
}

// counter-clockwise
Vector2 Vectors::Perpendicular(Vector3 a)
{
	return Vector2(-a.y, a.x);
}

float Vectors::Angle(Vector2 a, Vector2 b)
{
	return Dot(a, b) / (Length(a) * Length(b));
}
float Vectors::Angle(Vector3 a, Vector3 b)
{
	return Dot(a, b) / (Length(a) * Length(b));
}

std::string Vectors::VectorToString(const Vector2& vec)
{
	return std::string("(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ")");
}
std::string Vectors::VectorToString(const Vector3& vec)
{
	return std::string("(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ")");
}
std::string Vectors::VectorToString(const Vector4& vec)
{
	return std::string("(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ", " + std::to_string(vec.w) + ")");
}

Vector3 Vectors::MultiplyPoint(const Matrix4x4& m, const Vector3& p)
{
	Vector3 res;
	res.x = (m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z) + m[0][3];
	res.y = (m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z) + m[1][3];
	res.z = (m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z) + m[2][3];
	return res;
}

std::ostream& operator << (std::ostream& stream, const Vector2& vec)
{
	stream << "(" << vec.x << ", " << vec.y << ")";
	return stream;
}
std::ostream& operator << (std::ostream& stream, const Vector3& vec)
{
	stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return stream;
}
std::ostream& operator << (std::ostream& stream, const Vector4& vec)
{
	stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
	return stream;
}
std::ostream& operator << (std::ostream& stream, const Matrix4x4& mat)
{
	for (int x = 0; x < 4; x++, stream << std::endl)
		for (int y = 0; y < 4; y++)
			stream << std::setw(10) << mat[x][y] << " ";
	return stream;
}

Vector2& operator * (Vector2& vec, int value)
{
	vec.x *= value;
	vec.y *= value;
	return vec;
}
Vector2& operator * (Vector2& vec, float value)
{
	vec.x *= value;
	vec.y *= value;
	return vec;
}
Vector2& operator * (Vector2& vec, double value)
{
	vec.x *= value;
	vec.y *= value;
	return vec;
}
Vector3& operator * (Vector3& vec, int value)
{
	vec.x *= value;
	vec.y *= value;
	vec.z *= value;
	return vec;
}
Vector3& operator * (Vector3& vec, float value)
{
	vec.x *= value;
	vec.y *= value;
	vec.z *= value;
	return vec;
}
Vector3& operator * (Vector3& vec, double value)
{
	vec.x *= value;
	vec.y *= value;
	vec.z *= value;
	return vec;
}
Vector4& operator * (Vector4& vec, int value)
{
	vec.x *= value;
	vec.y *= value;
	vec.z *= value;
	vec.w *= value;
	return vec;
}
Vector4& operator * (Vector4& vec, float value)
{
	vec.x *= value;
	vec.y *= value;
	vec.z *= value;
	vec.w *= value;
	return vec;
}
Vector4& operator * (Vector4& vec, double value)
{
	vec.x *= value;
	vec.y *= value;
	vec.z *= value;
	vec.w *= value;
	return vec;
}