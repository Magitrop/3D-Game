#pragma once
#include "RendererComponentBase.h"
#include <vector>

class LineRendererComponent : public RendererComponentBase
{
	COMPONENT(LineRendererComponent, RendererComponentBase)
public:
	float lineWidth = 1.0f;
	bool connectLines = true;
	Color color = Color(1, 1, 1, 1);

	virtual void Render() override;

	void SetPoints(std::vector<Vector3>& points);
protected:
	virtual void OnCreate() override;

	std::vector<unsigned int> indices;
	unsigned int VAO;
	unsigned int vertexBuffer = 0;
	unsigned int trianglesBuffer = 0;
};