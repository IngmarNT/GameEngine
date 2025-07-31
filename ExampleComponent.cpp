#include "ExampleComponent.h"
#include "raylib-cpp.hpp"
#include <iostream>
#include <cmath>

ExampleComponent::ExampleComponent(GameObject& gM) : IComponent(gM) {};

void ExampleComponent::Initialize() 
{
	this->gameObject.AddComponent(shared_from_this());
	std::cout << "Initialized component @ " << shared_from_this().get() << std::endl;
}

void ExampleComponent::Start() 
{
	std::cout << "Ran Start() in component." << std::endl;
}

void ExampleComponent::Update2D() 
{
	static raylib::Camera& cam = Scene::GetActive().GetCamera();

	DrawText("Game Engine!", 10, 10, 40, BLACK);
	DrawFPS(GetScreenWidth() - 100, 10);
}

void ExampleComponent::Update3D() 
{
	static raylib::Camera& cam = Scene::GetActive().GetCamera();
	static GameTransform& t = gameObject.GetTransform();

	float deltaTime = GetFrameTime();
	float epsilon = 0.001f;
	float moveSpeed = 6.0f;

	Vec3 move(0, 0, 0);
	double frameMoveSpeed = moveSpeed * deltaTime;

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

	if (move != Vec3(0, 0, 0)) 
	{
		t.Translate(move);
	}

	if (Vec3(cam.target) != t.position) {
		Vec3 newTarget = Vec3::Lerp(cam.target, t.position, 10 * deltaTime);

		// if the result of the lerp is sufficiently close to the object position, snap the value
		if ((Vec3(cam.target) - newTarget).Magnitude() <= epsilon) newTarget = t.position;

		cam.SetTarget(newTarget);
	}
}