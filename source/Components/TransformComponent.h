#pragma once

#include "../Math/Vectors.h"
#include "Component.h"

class TransformComponent : public Component,
	CannotBeRemovedComponentAttribute,
	MustBeUniqueComponentAttribute
{
	COMPONENT(TransformComponent)
protected:
	Vector3 position = Vector3();
	Vector3 rotation = Vector3();
	Vector3 scale = Vector3(1);

	Vector3 localPosition = Vector3();
	Vector3 localRotation = Vector3();
	Vector3 localScale = Vector3(1);

	Vector3 right = Vector3(1, 0, 0);
	Vector3 up = Vector3(0, 1, 0);
	Vector3 forward = Vector3(0, 0, 1);
public:
	 Vector3 GetPosition() const;
	 Vector3 GetRotation() const;
	 Vector3 GetScale() const;
	 Vector3 GetLocalPosition() const;
	 Vector3 GetLocalRotation() const;
	 Vector3 GetLocalScale() const;

	 Vector3 GetForward() const;
	 Vector3 GetRight() const;
	 Vector3 GetUp() const;

	 Matrix4x4 GetModelMatrix() const;

	 const Vector3& SetPosition(const Vector3& pos);
	 const Vector3& SetPosition(float x, float y, float z);
	 const Vector3& SetLocalPosition(const Vector3& pos);
	 const Vector3& SetLocalPosition(float x, float y, float z);

	 const Vector3& SetRotation(const Vector3& rot);
	 const Vector3& SetRotation(float x, float y, float z);
	 const Vector3& SetLocalRotation(const Vector3& rot);
	 const Vector3& SetLocalRotation(float x, float y, float z);

	 const Vector3& SetScale(const Vector3& sc);
	 const Vector3& SetScale(float x, float y, float z);
	 const Vector3& SetLocalScale(const Vector3& sc);
	 const Vector3& SetLocalScale(float x, float y, float z);

	 const Vector3& SetForward(const Vector3& vec);
	 const Vector3& SetRight(const Vector3& vec);
	 const Vector3& SetUp(const Vector3& vec);

	 const Vector3& Translate(const Vector3& vec);
	 const Vector3& Translate(const Vector3& direction, float distance);
	 const Vector3& Rotate(const Vector3& axis, float angle);
	 const Vector3& Scale(const Vector3& sc);
};