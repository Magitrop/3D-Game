#pragma once

#include "GameObject.h"

class ObjectsManager final
{
private:
	std::list<GameObject> instantiatedObjects;
public:
	template<class T> static T* Instantiate()
	{
		static_assert(
			std::is_base_of<GameObject, T>::value ||
			std::is_base_of<Component, T>::value,
			"Only GameObject may be instantiated.");

		return GameObject::template CreateInstance<T>();
	}
};