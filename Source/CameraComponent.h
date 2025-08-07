#pragma once
#include "IComponent.h"
#include "raylib-cpp.hpp"
#include "MyEngineTypes.h"

class CameraComponent : public IComponent 
{
private:
	static inline CameraComponent* activeCamera = nullptr;
	raylib::Camera camera;
	MyEngine::GameTransform& transform;
public:
	CameraComponent(GameObject& gM);
	~CameraComponent();

	raylib::Camera& GetCamera();

	static CameraComponent* GetActive();
	void SetAsActive();

	void SetPosition(MyEngine::Vec3 pos);
	void SetRotation(MyEngine::Vec3 rot);
	void SetRotation(MyEngine::Quat rot);
	void SetTarget(MyEngine::Vec3 target);

	void Initialize() override;
	void Start() override;
	void FixedUpdate() override;
	void Update2D() override;
	void Update3D() override;
	void Destroy() override;
};