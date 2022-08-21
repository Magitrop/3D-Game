#pragma once
#include "../Controllers/Initializer.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include <memory>
#include <vector>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../GameObject/ObjectsManager.h"
#include "TransformComponent.h"
#include "RendererComponentBase.h"
#include "../Controllers/EventSystem.h"
#include "../Model.h"

class Model;
class Shader;
class ModelRendererComponent : public RendererComponentBase
{
	COMPONENT(ModelRendererComponent, RendererComponentBase)
protected:
	const Model* model = nullptr;
	const Shader* currentShader = nullptr;
public:
	void SetModel(const Model* newModel);
	void SetShader(const Shader* newShader);
	virtual void Render() override;
	void RenderDepth(const Shader* depthShader);
};