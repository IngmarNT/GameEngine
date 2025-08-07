#include "Scene.h"
#include "PhysicsManager.h"
#include "JSONParsing.h"
#include <iostream>

Scene::Scene() {}

Scene& Scene::GetActive() 
{
	static Scene active;
	return active;
}

void Scene::Start() 
{
	for (auto obj : sceneObjects)
	{
		obj->Start();
	}
}

void Scene::Update2D()
{
	for (auto obj : sceneObjects)
	{
		obj->Update2D();
	}
}

void Scene::Update3D() 
{
	for (auto obj : sceneObjects)
	{
		obj->Update3D();
	}
}

bool Scene::Open(nlohmann::json data) 
{
	if (data.contains("objects")) 
	{
		JSONParsing::GenerateSceneFromJSON(data);
		return true;
	}
	return false;
}

void Scene::Close() 
{
	for (size_t i = sceneObjects.size(); i > 0; i--) 
	{
		std::cout << "Destroying Object: " << sceneObjects[0]->GetName() << std::endl;
		sceneObjects[0]->Destroy();
	}
}

void Scene::FixedUpdate()
{
	const float physicsStep = 1.0f / 60.0f;
	const int maxStep = 6;
	static float accumulator = 0.0f;
	int step = 0;
	
	float deltaTime = GetFrameTime();
	accumulator += deltaTime;

	while (accumulator >= physicsStep && step < maxStep) 
	{
		for (auto obj : sceneObjects)
		{
			obj->GetTransform().SavePrevious();
			obj->FixedUpdate();
		}

		PhysicsManager::Get().Update(physicsStep);

		step++;
		accumulator -= physicsStep;
	}

	physicsAlpha = std::clamp(accumulator / physicsStep, 0.0f, 1.0f);

	for (auto obj : sceneObjects)
	{
		auto physics = obj->GetComponent<PhysicsComponent>();
		if (physics)
		{
			physics->SyncFromTransform();
		}
	}
}

std::shared_ptr<GameObject> Scene::FindObject(std::string name) 
{
	for (auto obj : sceneObjects) 
	{
		if (obj->GetName() == name) 
		{
			return obj;
		}
	}

	return nullptr;
}

bool Scene::CheckInScene(std::shared_ptr<GameObject> obj) 
{
	for (auto o : sceneObjects)
	{
		if (o.get() == obj.get()) return true;
	}
	return false;
}

bool Scene::AddObject(std::shared_ptr<GameObject> obj) 
{
	for (auto o : sceneObjects) 
	{
		if (o.get() == obj.get()) return false;
	}
	sceneObjects.push_back(obj);
	return true;
}

bool Scene::RemoveObject(std::shared_ptr<GameObject> obj) 
{
	if (this->sceneObjects.empty()) return false;

	for (auto it = this->sceneObjects.begin(); it != this->sceneObjects.end(); ++it)
	{
		if ((*it) == obj)
		{
			sceneObjects.erase(it);
			return true;
		}
	}
	return false;
}

float Scene::GetPhysicsAlpha() { return this->physicsAlpha; }