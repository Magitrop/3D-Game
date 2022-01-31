#pragma once
#include <map>
#include <list>
#include <string>
#include <type_traits>

#include "Component.h"
#include "../GameObject/ObjectsManager.h"
#include "../GameObject/GameObject.h"

class GameObject;
class ComponentsManager final
{
private:
	friend class Component;
	friend class GameObject;

	static std::map<GameObject*, std::map<const std::string, std::list<Component>>> componentsPool;
public:
	template<class T, class = 
		std::enable_if<
		std::is_base_of<Component, T>::value>::type> 
			static void AddComponent(GameObject* addTo, bool allowRepeatUnique = false)
	{
		std::string componentTypeName = typeid(T).name();
		auto& obj = componentsPool[addTo];

		if (!allowRepeatUnique &&
			std::is_base_of<MustBeUniqueComponentAttribute, T>::value &&
			obj.find(componentTypeName) != obj.end())
		{
			std::cerr << "Cannot add another \"" << componentTypeName << "\" component." << std::endl;
			return;
		}

		obj[componentTypeName].emplace_back(addTo);
	}

	template<class T, class =
		std::enable_if<
		std::is_base_of<Component, T>::value>::type>
		static T* GetComponent(GameObject* getFrom, int withIndex = 0)
	{
		std::string componentTypeName = typeid(T).name();
		auto obj = componentsPool[getFrom];

		if (obj.find(componentTypeName) == obj.end() ||
			obj[componentTypeName].size() <= withIndex)
			return nullptr;

		auto comp = obj[componentTypeName].begin();
		std::advance(comp, withIndex);
		return static_cast<T*>(&*comp);
	}
};