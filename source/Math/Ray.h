#pragma once
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>

using glm::Vector3;

class Ray
{
public:
	Ray(const Vector3& start = Vector3(0), const Vector3& end = Vector3(0)) : origin(start), direction(glm::normalize(end)) {}

	const Vector3 origin;
	const Vector3 direction;

	Vector3 GetPoint(float distance) const;
};