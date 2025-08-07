#include "ControllerComponent.h"
#include "CameraComponent.h"
#include "PhysicsManager.h"
#include "raylib-cpp.hpp"
#include <iostream>
#include <cmath>

ControllerComponent::ControllerComponent(GameObject& gM) : IComponent(gM) {}

void ControllerComponent::Initialize() 
{
	this->gameObject.AddComponent(shared_from_this());
}

void ControllerComponent::Start() 
{
	if (auto p = gameObject.GetComponent<PhysicsComponent>()) 
	{
		pComp = p;
		jumpQueued = false;
		move = MyEngine::Vec3();
	}
}

void ControllerComponent::FixedUpdate() 
{
	static auto& bodyInterface = PhysicsManager::Get().GetSystem().GetBodyInterface();
	if (!pComp) return;
	JPH::BodyID id = pComp->GetBodyID();

	if (move != MyEngine::Vec3(0, 0, 0))
	{
		PhysicsManager::Get().GetSystem().GetBodyInterface().AddForce(id, JPH::Vec3(move.x, move.y, move.z));
	}

	if (jumpQueued) 
	{
		bodyInterface.ActivateBody(id);
		bodyInterface.AddImpulse(id, JPH::Vec3(0.0f, 10.0f, 0.0f));
		jumpQueued = false;
	}
}

void ControllerComponent::Update2D() 
{

}

void ControllerComponent::Update3D() 
{
	static CameraComponent* camComp = CameraComponent::GetActive();
	static MyEngine::GameTransform& t = gameObject.GetTransform();

	float epsilon = 0.001f;
	float moveSpeed = 20.0f;

	MyEngine::Vec3 _move(0, 0, 0);

	if (IsKeyDown('A')) { _move.x += 1; }
	if (IsKeyDown('D')) { _move.x += -1; }
	if (IsKeyDown('W')) { _move.z += 1; }
	if (IsKeyDown('S')) { _move.z += -1; }

	if (_move.Magnitude() > 0.0f)
	{
		_move = _move.Normalized();
	}

	_move *= moveSpeed;
	move = _move;

	if (IsKeyPressed(KEY_SPACE))
	{
		jumpQueued = true;
	}

	float deltaTime = GetFrameTime();
	float alpha = Scene::GetActive().GetPhysicsAlpha();

	MyEngine::Vec3 interpPos = t.GetInterpolatedPosition(alpha);

	if (camComp == nullptr) return;

	raylib::Camera& cam = camComp->GetCamera();
	if (MyEngine::Vec3(cam.target) != interpPos) {
		MyEngine::Vec3 newTarget = MyEngine::Vec3::Lerp(cam.target, interpPos, 10 * deltaTime);

		if ((MyEngine::Vec3(cam.target) - newTarget).Magnitude() <= epsilon) newTarget = interpPos;

		camComp->SetTarget(newTarget);
	}
}

void ControllerComponent::Destroy() 
{
	this->gameObject.RemoveComponent(shared_from_this());
}