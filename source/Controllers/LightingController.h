#pragma once
#include <glm\ext\matrix_float4x4.hpp>
#include <vector>
#include "../Components/ModelRendererComponent.h"

using glm::Matrix4x4;

class Shader;
class LightingController final
{
private:
	LightingController() = delete;

	static unsigned int depthMapFBO;
	static unsigned int SHADOW_WIDTH;
	static unsigned int SHADOW_HEIGHT;
	static unsigned int depthMap;

	static Matrix4x4 lightProjection;
	static Matrix4x4 lightView;
	static Matrix4x4 lightSpaceMatrix;
	static GLint lightSpaceMatrixID;

	static void RecalculateDepthMap();
public:
	static Vector3 lightPos;
	static Vector3 lightRot;

	static const Shader* depthShader;
	static void Initialize();
	static void SetShadowMapScale(unsigned int width, unsigned int height);
	static void PrepareDepthMap(/*std::vector<ModelRendererComponent*> meshesWithShadows*/);
	static GLuint GetDepthMapID();
	static Matrix4x4 GetLightProjectionMatrix();
	static Matrix4x4 GetLightViewMatrix();
	static Matrix4x4 GetLightSpaceMatrix();
};