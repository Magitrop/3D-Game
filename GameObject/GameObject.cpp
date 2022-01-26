#include "GameObject.h"

GameObject::GameObject() : name("New Object")
{
	// every game object must contain irremovable transform component
	AddComponent<TransformComponent>();
}