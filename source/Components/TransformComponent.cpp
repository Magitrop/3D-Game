#pragma once
#include "TransformComponent.h"
#include "../Controllers/EventSystem.h"

#include <math.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Vector3 TransformComponent::GetPosition() const { return position; }
Vector3 TransformComponent::GetRotation() const { return rotation; }
Vector3 TransformComponent::GetScale() const { return scale; }

Vector3 TransformComponent::GetForward() const { return forward; }
Vector3 TransformComponent::GetRight() const { return right; }
Vector3 TransformComponent::GetUp() const { return up; }

Matrix4x4 TransformComponent::GetModelMatrix() const
{
	Matrix4x4 res(1.f);
	res = glm::scale(res, GetScale());
	res = glm::rotate(res, glm::radians(GetRotation().z), Vectors::forward/*GetForward()*/);
	res = glm::rotate(res, glm::radians(GetRotation().y), Vectors::up/*GetUp()*/);
	res = glm::rotate(res, glm::radians(GetRotation().x), Vectors::right/*GetRight()*/);
	res = glm::translate(res, GetPosition());
	return res;
}
const Matrix4x4& TransformComponent::GetMVPMatrix()
{
	MVP = 
		EventSystem::GetMainCamera()->GetProjectionMatrix() *
		EventSystem::GetMainCamera()->GetViewMatrix() *
		GetModelMatrix();
	return MVP;
}

const Vector3& TransformComponent::SetPosition(const Vector3& vec) { return position = vec; }
const Vector3& TransformComponent::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	return position;
}

const Vector3& TransformComponent::SetRotation(const Vector3& vec) { return rotation = vec; }
const Vector3& TransformComponent::SetRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
	return rotation;
}

const Vector3& TransformComponent::SetScale(const Vector3& vec) { return scale = vec; }
const Vector3& TransformComponent::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
	return scale;
}

const Vector3& TransformComponent::SetForward(const Vector3& vec)
{
	forward = glm::normalize(vec);
	right = -glm::normalize(glm::cross(vec, Vector3(0, 1, 0)));
	up = glm::normalize(glm::cross(forward, right));
	return forward;
}
const Vector3& TransformComponent::SetRight(const Vector3& vec)
{
	right = glm::normalize(vec);
	forward = glm::normalize(glm::cross(vec, Vector3(0, 1, 0)));
	up = glm::normalize(glm::cross(forward, right));
	return forward;
}
const Vector3& TransformComponent::SetUp(const Vector3& vec)
{
	up = glm::normalize(vec);
	right = glm::normalize(glm::cross(vec, Vector3(0, 0, 1)));
	forward = -glm::normalize(glm::cross(up, right));
	return forward;
}

const Vector3& TransformComponent::Translate(const Vector3& vec) { return position += vec; }
const Vector3& TransformComponent::Translate(const Vector3& direction, float distance)
{
	return position += glm::normalize(direction) * distance;
}
const Vector3& TransformComponent::Rotate(const Vector3& axis, float angle)
{
	// signum function substitution
	rotation.x += angle * ((axis.x > 0) - (axis.x < 0));
	rotation.y += angle * ((axis.y > 0) - (axis.y < 0));
	rotation.z += angle * ((axis.z > 0) - (axis.z < 0));
	std::cout << rotation << std::endl;
	return rotation;
}
const Vector3& TransformComponent::Scale(const Vector3& sc)
{
	return scale += sc;
}