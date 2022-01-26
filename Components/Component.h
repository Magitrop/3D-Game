#pragma once
#include <iostream>
#include <string>

class GameObject;
class Component
{
private:
	void operator delete (void*) {}
	Component& operator = (const Component&) = delete;
protected:
public:
	Component(GameObject* owner) : gameObject(owner) {}

	virtual ~Component() {}

	GameObject * const gameObject;
};

class CannotBeRemovedComponentAttribute {}; 
class MustBeUniqueComponentAttribute {};