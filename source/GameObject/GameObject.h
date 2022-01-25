#pragma once
#include <iostream>
#include <map>
#include <list>
#include <memory>
#include <type_traits>
#include <concepts>
#include <string>
#include <optional>

#define _USE_MATH_DEFINES
#include <cmath>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Components/Component.h"
#include "../Components/TransformComponent.h"
#include "../Math/Vectors.h"
#include "ObjectsManager.h"

class TransformComponent;
class GameObject final
{
private:
	friend class ObjectsManager;

	void operator delete (void*) {}
	GameObject(const GameObject& other) = delete;

	GameObject() : name("New Object")
	{
		// every game object must contain irremovable transform component
		AddComponent<TransformComponent>();
	}

	template<class T> static T* CreateInstance()
	{
		static_assert(std::is_base_of<Component, T>::value, "Only GameObject may be instantiated.");

		GameObject* obj = new GameObject();
		obj->template AddComponent<T>();
		return obj->template GetComponent<T>();
	}
	template<> static GameObject* CreateInstance<GameObject>()
	{
		return new GameObject();
	}

	std::map<const std::string, std::list<Component>> components;
public:
	GameObject(GameObject&&) = default;
	~GameObject()
	{
		std::cout << "destroyed game object" << std::endl;
	}

	std::string name;

	template<class T> void AddComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "Type must be inherited from Component.");

		std::string componentTypeName = typeid(T).name();

		if (std::is_base_of<MustBeUniqueComponentAttribute, T>::value &&
			components.find(componentTypeName) != components.end())
		{
			std::cerr << "Cannot add another \"" << componentTypeName << "\" component." << std::endl;
			return;
		}

		components[componentTypeName].emplace_back(this);
	}
	template<class T> void RemoveComponent(int withIndex = 0)
	{
		static_assert(std::is_base_of<Component, T>::value, "Type must be inherited from Component.");
		static_assert(!std::is_base_of<CannotBeRemovedComponentAttribute, T>::value, "This Component cannot be removed.");

		std::string componentTypeName = typeid(T).name();

		if (components.find(componentTypeName) == components.end() ||
			components[componentTypeName].size() <= withIndex)
		{
			std::cerr << "The requested component \"" << componentTypeName << "\" was not found." << std::endl;
			return;
		}

		auto at = components[componentTypeName].begin();
		std::advance(at, withIndex);
		components[componentTypeName].erase(at);

		if (components[componentTypeName].size() == 0)
			components.erase(componentTypeName);
	}

	template<class T> T* GetComponent(int withIndex = 0)
	{
		static_assert(std::is_base_of<Component, T>::value, "Type must be inherited from Component.");

		std::string componentTypeName = typeid(T).name();

		if (components.find(componentTypeName) == components.end() ||
			components[componentTypeName].size() <= withIndex)
			return nullptr;

		auto comp = components[componentTypeName].begin();
		std::advance(comp, withIndex);
		return static_cast<T*>(&*comp);
	}
};