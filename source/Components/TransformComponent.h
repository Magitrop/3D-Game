#pragma once

#include <memory>

#include "Component.h"
#include "../GameObject/GameObject.h"
#include "../Math/Vectors.h"

#define Transform TransformComponent

class TransformComponent : public Component, 
	CannotBeRemovedComponentAttribute,
	MustBeUniqueComponentAttribute
{
private:
protected:
public:
	TransformComponent(GameObject* owner) : Component(owner) {}

	Vector3 position = Vector3();
	Vector3 rotation = Vector3();
	Vector3 scale = Vector3(1);
};

class SampleComponent : public Component, 
	MustBeUniqueComponentAttribute
{
private:
protected:
public:
	SampleComponent(GameObject* owner) : Component(owner) {}

	Vector3 position = Vector3();
	Vector3 rotation = Vector3();
	Vector3 scale = Vector3(1);
};