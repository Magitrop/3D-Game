#pragma once
#include "RendererComponentBase.h"
#include <vector>

class LineRendererComponent : public RendererComponentBase
{
	COMPONENT(LineRendererComponent, RendererComponentBase)
public:
	bool connectLines = true;

	virtual void Render() override;
	virtual void OnCreate() override;

	void SetPoints(std::vector<Vector3>& points);
private:
	std::vector<unsigned int> indices;
	unsigned int VAO;
	unsigned int vertexBuffer = 0;
	unsigned int trianglesBuffer = 0;
};