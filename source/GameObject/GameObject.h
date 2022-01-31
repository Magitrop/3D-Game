#pragma once

#include "../Components/Component.h"
#include "../Components/TransformComponent.h"

#include <map>
#include <string>
#include <list>
#include <typeinfo>
#include <iostream>

class GameObject final 
{
    friend class ObjectsManager;
    friend class EventSystem;

    std::map<std::string, Component*> components;
    explicit GameObject();

    void operator delete(void* ptr)
    {
        ::operator delete(ptr);
    }

    void operator delete[](void* ptr)
    {
        ::operator delete[](ptr);
    }
public:
    std::string name;
    TransformComponent* const transform;

    ~GameObject() 
    {
        for (const auto&[componentName, component] : components)
        {
            delete component;
        }
        components.clear();
    }

    template<class T, class =
        std::enable_if<
        std::is_base_of<Component, T>::value>::type>
        T* AddComponent()
    {
        if (components.find(typeid(T).name()) != components.end())
            return GetComponent<T>();

        return static_cast<T*>(components[typeid(T).name()] = new T(this));
    }

    template<class T, class =
        std::enable_if<
        std::is_base_of<Component, T>::value>::type>
        T* GetComponent()
    {
        if (components.find(typeid(T).name()) == components.end()) 
            return nullptr;

        return static_cast<T*>(components[typeid(T).name()]);
    }

    template<class T, class =
        std::enable_if<
        std::is_base_of<Component, T>::value>::type>
        void RemoveComponent()
    {
        auto componentName = typeid(T).name();
        if (components.find(componentName) == components.end()) 
            return;

        delete components[componentName];
        components.erase(componentName);
    }
};