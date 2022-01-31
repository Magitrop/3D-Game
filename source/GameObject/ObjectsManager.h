#pragma once

#include <list>

#include "GameObject.h"

class ObjectsManager final
{
private:
	ObjectsManager() = delete;

	static std::list<GameObject*> instantiatedObjects;
public:
	template<class T> static T* Instantiate()
	{
		static_assert(std::is_base_of<Component, T>::value, "Only GameObject may be instantiated.");

		GameObject* obj = new GameObject();
		instantiatedObjects.push_back(obj);
		return obj->AddComponent<T>();
	}

	template<> static GameObject* Instantiate<GameObject>()
	{
		GameObject* obj = new GameObject();
		instantiatedObjects.push_back(obj);
		return obj;
	}

	static void DestroyObject(GameObject* toDestroy)
	{
		auto it = std::find(instantiatedObjects.begin(), instantiatedObjects.end(), toDestroy);
		if (it != instantiatedObjects.end())
		{
			delete toDestroy;
			instantiatedObjects.erase(it);
		}
	}

	template<class T, class =
		std::enable_if<
		std::is_base_of<Component, T>::value>::type>
		static T* FindObjectOfType()
	{
		for (auto it = instantiatedObjects.begin(); it != instantiatedObjects.end(); it++)
			if ((*it)->GetComponent<T>())
				return *it;
		return nullptr;
	}
};