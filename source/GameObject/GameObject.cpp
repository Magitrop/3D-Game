#include "GameObject.h"
#include "../Components/TransformComponent.h"

// every game object must contain irremovable transform component
GameObject::GameObject() : name("New Object"), transform(AddComponent<TransformComponent>()) {}