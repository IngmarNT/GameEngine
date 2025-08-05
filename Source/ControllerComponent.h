#pragma once
#include "IComponent.h"
#include "GameObject.h"

class ControllerComponent : public IComponent 
{
private:
	std::shared_ptr<PhysicsComponent> pComp;
	MyEngine::Vec3 move;
	bool jumpQueued;
public:
	ControllerComponent(GameObject& gM);

	void Initialize() override;
	void Start() override;
	void Update2D() override;
	void Update3D() override;
	void FixedUpdate() override;
	void Destroy() override;
};