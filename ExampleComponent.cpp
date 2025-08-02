#include "ExampleComponent.h"
#include "PhysicsManager.h"
#include "raylib-cpp.hpp"
#include <iostream>
#include <cmath>

ExampleComponent::ExampleComponent(GameObject& gM) : IComponent(gM) {}

void ExampleComponent::Initialize() 
{
	this->gameObject.AddComponent(shared_from_this());
}

void ExampleComponent::Start() 
{
	if (auto p = gameObject.GetComponent<PhysicsComponent>()) 
	{
		pComp = p;
	}
}

void ExampleComponent::FixedUpdate() 
{
	static raylib::Camera& cam = Scene::GetActive().GetCamera();
	static MyEngine::GameTransform& t = gameObject.GetTransform();
	static auto& bodyInterface = PhysicsManager::Get().GetSystem().GetBodyInterface();
	if (!pComp) return;
	JPH::BodyID id = pComp->GetBodyID();

	float deltaTime = GetFrameTime();
	float epsilon = 0.001f;
	float moveSpeed = 20.0f;

	MyEngine::Vec3 move(0, 0, 0);
	double frameMoveSpeed = moveSpeed;

	if (IsKeyDown('A')) { move.x += frameMoveSpeed; }
	if (IsKeyDown('D')) { move.x += -frameMoveSpeed; }
	if (IsKeyDown('W')) { move.z += frameMoveSpeed; }
	if (IsKeyDown('S')) { move.z += -frameMoveSpeed; }

	float mag = move.Magnitude();

	if (mag != frameMoveSpeed && mag != 0)
	{
		float diff = frameMoveSpeed / mag;

		move *= diff;
	}

	if (IsKeyPressed(KEY_SPACE))
	{
		bodyInterface.ActivateBody(id);
		bodyInterface.AddImpulse(id, JPH::Vec3(0.0f, 10.0f, 0.0f));
	}

	if (move != MyEngine::Vec3(0, 0, 0))
	{
		PhysicsManager::Get().GetSystem().GetBodyInterface().AddForce(id, JPH::Vec3(move.x, move.y, move.z));
	}

	if (MyEngine::Vec3(cam.target) != t.position) {
		MyEngine::Vec3 newTarget = MyEngine::Vec3::Lerp(cam.target, t.position, 10 * deltaTime);

		// if the result of the lerp is sufficiently close to the object position, snap the value
		if ((MyEngine::Vec3(cam.target) - newTarget).Magnitude() <= epsilon) newTarget = t.position;

		cam.SetTarget(newTarget);
	}
}

void ExampleComponent::Update2D() 
{
	//DrawText("Game Engine!", 10, 10, 40, BLACK);
	DrawFPS(GetScreenWidth() - 100, 10);
}

void ExampleComponent::Update3D() 
{
	
}