#include "Ray.h"

Vector3 Ray::GetPoint(float distance) const
{
	return origin + glm::normalize(direction) * distance;
}