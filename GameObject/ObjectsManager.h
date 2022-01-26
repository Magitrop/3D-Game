#pragma once

#include "GameObject.h"

class GameObject;
class ObjectsManager final
{
private:
	static std::list<GameObject*> instantiatedObjects;
public:
	template<class T> static T* Instantiate()
	{
		static_assert(std::is_base_of<Component, T>::value, "Only GameObject may be instantiated.");

		auto obj = GameObject::template CreateInstance<T>();
		instantiatedObjects.push_back(obj->gameObject);
		return obj;
	}

	template<> static GameObject* Instantiate<GameObject>()
	{
		GameObject* obj = GameObject::CreateInstance<GameObject>();
		instantiatedObjects.push_back(obj);
		return obj;
	}
};