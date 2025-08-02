#pragma once
#include "IComponent.h"
#include "GameObject.h"

class ExampleComponent : public IComponent 
{
private:
	std::shared_ptr<PhysicsComponent> pComp;
public:
	ExampleComponent(GameObject& gM);

	void Initialize() override;
	void Start() override;
	void Update2D() override;
	void Update3D() override;
	void FixedUpdate() override;
};