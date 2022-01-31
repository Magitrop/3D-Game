#include "Vectors.h"

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

//Vector3 Vectors::MultiplyPoint(const Matrix4x4& m, const Vector3& p)
//{
//	Vector3 res;
//	res.x = (m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z) + m[0][3];
//	res.y = (m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z) + m[1][3];
//	res.z = (m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z) + m[2][3];
//	return res;
//}

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