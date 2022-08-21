#include "ObjectsManager.h"
std::list<GameObject*> ObjectsManager::instantiatedObjects;
std::list<RendererComponentBase*> ObjectsManager::renderQueue;