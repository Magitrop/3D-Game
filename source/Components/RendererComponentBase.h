#pragma once
#include "Component.h"

class RendererComponentBase : public Component
{
	COMPONENT(RendererComponentBase, Component)
protected:
	virtual void OnCreate() override;
public:
	virtual void Render() = 0;
};