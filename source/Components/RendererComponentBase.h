#pragma once
#include "Component.h"

class RendererComponentBase : public Component
{
	COMPONENT(RendererComponentBase, Component)
protected:
	virtual void OnDestroy() override;
	virtual void OnCreate() override;
public:
	bool castShadows = true;
	bool receiveShadows = true;
	bool useDepthBuffer = true;

	virtual void Render() = 0;
	virtual void RenderDepth() {}
};