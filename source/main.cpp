#define _USE_MATH_DEFINES
#include <cmath>

#ifdef __gl_h_
#undef __gl_h_
#endif // __gl_h_
#include <glad/glad.h>

#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "PerlinNoise.hpp"
#include "Controllers/Initializer.h"
#include "Controllers/EventSystem.h"
#include "Controllers/LightingController.h"
#include "GameObject/ObjectsManager.h"
#include "Math/Vectors.h"
#include "Shaders/Shader.h"
#include "Components/ModelRendererComponent.h"
#include "Components/TextRendererComponent.h"
#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"
#include "Controllers/ResourceManager.h"
#include "Components/LineRendererComponent.h"

#include "BulletCollision/BroadphaseCollision/btBroadphaseInterface.h"
#include "BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"
#include "btBulletCollisionCommon.h"
#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using glm::Matrix4x4;

void LoadShaders()
{
	ResourceManager::UploadShader(Shader("..\\source\\TextVertexShader.vertexshader", "..\\source\\TextFragmentShader.fragmentshader"), "Text");
	ResourceManager::UploadShader(Shader("..\\source\\SimpleVertexShader.vertexshader", "..\\source\\SimpleFragmentShader.fragmentshader"), "Simple");
	//Shader::UploadShader(Shader("..\\source\\TextureVertShader.vertexshader", "..\\source\\TextureFragmentShader.fragmentshader"), "Map");
	ResourceManager::UploadShader(Shader("..\\source\\DepthShader.vertexshader", "..\\source\\DepthFragShader.fragmentshader"), "Depth");
	ResourceManager::UploadShader(Shader("..\\source\\ShadowsVert.vertexshader", "..\\source\\ShadowsFrag.fragmentshader"), "Shadows");
}

void LoadModels()
{
	ResourceManager::UploadModel(Model("C:/Users/Schweppes_psina/Desktop/castle.fbx"), "Castle");
}

int main()
{
	if (!Initializer.Init())
		return -1;

	auto camera = ObjectsManager::Instantiate<CameraComponent>(Vector3(0, 0, -3));
	EventSystem::SetAsMainCamera(camera);
	LoadShaders();
	LoadModels();

	LightingController::depthShader = &ResourceManager::GetShader("Depth");
	LightingController::lightPos = Vector3(-1, 4, -1);
	LightingController::Initialize();

	std::vector<Texture> textures = { };
	ModelRendererComponent* meshObj1 = ObjectsManager::Instantiate<ModelRendererComponent>();
	meshObj1->SetModel(&ResourceManager::GetModel("Castle"));
	meshObj1->SetShader(&ResourceManager::GetShader("Shadows"));
	meshObj1->gameObject->transform->Scale({ 0.001f, 0.001f, 0.001f });
	meshObj1->gameObject->transform->Translate({ 1, 0, 4 });

	std::vector<Vertex> vertices;
	std::vector<unsigned int> triangles;

	const siv::PerlinNoise::seed_type seed = 123456u;
	const siv::PerlinNoise perlin{ seed };

	int sizeX = 30,
		sizeZ = 30;
	float scale = 0.05f;

	vertices.reserve((sizeX + 1) * (sizeZ + 1));
	std::map<std::pair<int, int>, std::vector<Vertex*>> tilePositions;
	for (int z = 0; z <= sizeZ; z++)
		for (int x = 0; x <= sizeX; x++)
		{
			vertices.push_back(Vector3(x * scale, 0, z * scale));
			Vertex* v = &vertices[vertices.size() - 1];
			tilePositions[std::make_pair(x / 2, z / 2)].push_back(v);
		}
	float height = 1.f;//perlin.octave2D_01(x * 0.1, z * 0.1f, 4) * 3;
	for (auto v : tilePositions[std::make_pair(2, 2)])
		v->Position.y = height;
	/*for (auto p : tilePositions)
	{
		auto& [x, z] = p.first;
		float height = -(x * z) * 0.01f;//perlin.octave2D_01(x * 0.1, z * 0.1f, 4) * 3;
		for (auto v : p.second)
			v->Position.y = height;
	}*/
	triangles.resize(sizeX * sizeZ * 6);
	for (int ti = 0, vi = 0, z = 0; z < sizeZ; z++, vi++)
	{
		for (int x = 0; x < sizeX; x++, ti += 6, vi++)
		{
			triangles[ti] = vi;
			triangles[ti + 3] = triangles[ti + 2] = vi + 1;
			triangles[ti + 4] = triangles[ti + 1] = vi + sizeX + 1;
			triangles[ti + 5] = vi + sizeX + 2;
		}
	}
	ModelRendererComponent* meshObj2 = ObjectsManager::Instantiate<ModelRendererComponent>();
	meshObj2->SetModel(new Model({ new Mesh(vertices, triangles, textures) }));
	meshObj2->SetShader(&ResourceManager::GetShader("Shadows"));
	meshObj2->gameObject->transform->Scale(Vector3(10, 1, 10));

	LineRendererComponent* line = ObjectsManager::Instantiate<LineRendererComponent>();
	line->gameObject->transform->Translate({ 1, 0, 4 });

	TextRendererComponent* text = ObjectsManager::Instantiate<TextRendererComponent>();
	text->text = "abc";
	text->gameObject->transform->Translate({ 0.f, 100.f, 0.f });

	/*btBroadphaseInterface* broadphase = new btDbvtBroadphase();

	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));

	for (int x = 0; x < 15; x++)
		for (int z = 0; z < 15; z++)
		{
			btCollisionShape* boxCollisionShape = new btBoxShape(btVector3(0.5f, 0.05f, 0.5f));

			btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(
				btQuaternion(0, 0, 0, 1),
				btVector3(x + 0.5f, 0, z + 0.5f)
			));

			btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
				0,
				motionstate,
				boxCollisionShape,
				btVector3(0, 0, 0)
			);
			btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);

			dynamicsWorld->addRigidBody(rigidBody);
		}*/
	
	//rigidBody->setUserPointer((void*)meshObj1->gameObject);

	LineRendererComponent* grid = ObjectsManager::Instantiate<LineRendererComponent>();
	grid->connectLines = false;
	std::vector<Vector3> points;
	for (int x = 0; x < 16; x++)
	{
		points.push_back(Vector3(x, 0.01f, 0));
		points.push_back(Vector3(x, 0.01f, 15));
	}
	for (int z = 0; z < 16; z++)
	{
		points.push_back(Vector3(0, 0.01f, z));
		points.push_back(Vector3(15, 0.01f, z));
	}
	grid->SetPoints(points);

	line->color = Color(1, 0, 0, 1);
	line->lineWidth = 3;
	line->useDepthBuffer = false;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(Initializer.window, true);
	ImGui_ImplOpenGL3_Init(NULL);
	ImGui::StyleColorsDark();

	//ObjectsManager::DestroyObject(meshObj2->gameObject);
	while (!glfwWindowShouldClose(Initializer.window))
	{
		//if (EventSystem::GetMouseButtonClick(GLFW_MOUSE_BUTTON_LEFT))
		{
			/*Ray r = camera->ScreenPointToRay(EventSystem::GetMousePosition());
			Vector3 out_end = r.origin + r.direction * 1000.0f;

			btCollisionWorld::ClosestRayResultCallback RayCallback(
				btVector3(r.origin.x, r.origin.y, r.origin.z),
				btVector3(out_end.x, out_end.y, out_end.z)
			);
			dynamicsWorld->rayTest(
				btVector3(r.origin.x, r.origin.y, r.origin.z),
				btVector3(out_end.x, out_end.y, out_end.z),
				RayCallback
			);

			if (RayCallback.hasHit())
			{
				auto& v = RayCallback.m_collisionObject->getWorldTransform().getOrigin();
				Vector3 p(v.x(), v.y(), v.z());
				line->SetPoints(std::vector<Vector3> {
					p + Vector3(0.5f, 0.02f, -0.5f),
					p + Vector3(0.5f, 0.02f, 0.5f),
					p + Vector3(-0.5f, 0.02f, 0.5f),
					p + Vector3(-0.5f, 0.02f, -0.5f),
					p + Vector3(0.5f, 0.02f, -0.5f),
				});
			}*/
		}
		LightingController::PrepareDepthMap();

		glViewport(0, 0, Initializer.windowSize.x, Initializer.windowSize.y);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		static ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		ImGui::SetNextWindowPos(ImVec2(10, 10));
		ImGui::SetNextWindowSize(ImVec2(200, 200));
		if (ImGui::Begin("Menu", nullptr, flags))
		{
			ImGui::PushItemWidth(ImGui::GetWindowSize().x * 0.5f);
			ImGui::Button("Hello", ImVec2(-1.0f, 0.0f));
		}
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		text->RenderText();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, LightingController::GetDepthMapID());
		ResourceManager::GetShader("Shadows").SetInt("shadowMap", 0);

		for (auto& obj : ObjectsManager::renderQueue)
			if (obj->useDepthBuffer)
				obj->Render();
		glClear(GL_DEPTH_BUFFER_BIT);
		for (auto& obj : ObjectsManager::renderQueue)
			if (!obj->useDepthBuffer)
				obj->Render();

		EventSystem::Update();
		glfwSwapBuffers(Initializer.window);
		glfwPollEvents();

		for (long long i = 0; i < 1999999; ++i);
	}

	Initializer.Quit();
	return 0;
}