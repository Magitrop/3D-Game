#include <iostream>

#include "RendererComponentBase.h"
#include "../GameObject/ObjectsManager.h"

void RendererComponentBase::OnDestroy()
{
	auto it = std::find(ObjectsManager::renderQueue.begin(), ObjectsManager::renderQueue.end(), this);
	if (it != ObjectsManager::renderQueue.end())
		ObjectsManager::renderQueue.erase(it);
}

void RendererComponentBase::OnCreate()
{
	ObjectsManager::renderQueue.push_back(this);
}
