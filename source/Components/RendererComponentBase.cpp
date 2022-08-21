#include <iostream>

#include "RendererComponentBase.h"
#include "../GameObject/ObjectsManager.h"

void RendererComponentBase::OnCreate()
{
	ObjectsManager::renderQueue.push_back(this);
}
