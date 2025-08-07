#include "CameraComponent.h"
#include "GameObject.h"
#include <iostream>

CameraComponent::CameraComponent(GameObject& gM) : transform(gM.GetTransform()), IComponent(gM) {}

CameraComponent::~CameraComponent() 
{
	if (activeCamera == this)
	{
		activeCamera = nullptr;
	}
}

raylib::Camera& CameraComponent::GetCamera() 
{
	return camera;
}

void CameraComponent::SetAsActive() 
{
	activeCamera = this;
}

void CameraComponent::Initialize() 
{
	this->gameObject.AddComponent(shared_from_this());

	if (activeCamera == nullptr)
	{
		activeCamera = this;
	}
}

CameraComponent* CameraComponent::GetActive() 
{
	if (activeCamera == nullptr) std::cerr << "No active CameraComponent in scene!" << std::endl;
	return activeCamera;
}

void CameraComponent::SetPosition(MyEngine::Vec3 pos) 
{
	gameObject.GetTransform().position = pos;
	camera.position = pos;
}

void CameraComponent::SetRotation(MyEngine::Vec3 rot) 
{
	this->SetRotation(MyEngine::Quat::FromEuler(rot));
}

void CameraComponent::SetRotation(MyEngine::Quat rot) 
{
	transform.rotation = rot;
	camera.target = transform.GetForward();
}

void CameraComponent::SetTarget(MyEngine::Vec3 target) 
{
	this->camera.SetTarget(target);

	MyEngine::Vec3 dir = (target - transform.position).Normalized();

	float pitch = asinf(-dir.y);
	float yaw = atan2f(dir.x, dir.z);

	transform.rotation = MyEngine::Quat::FromEuler(pitch, yaw, 0.0f);
}

void CameraComponent::Start() {}
void CameraComponent::FixedUpdate() {}
void CameraComponent::Update2D() {}
void CameraComponent::Update3D() {}

void CameraComponent::Destroy() 
{
	this->gameObject.RemoveComponent(shared_from_this());
}