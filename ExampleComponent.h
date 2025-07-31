#pragma once
#include "IComponent.h"

class ExampleComponent : public IComponent 
{
public:
	ExampleComponent(GameObject& gM);

	void Initialize() override;
	void Start() override;
	void Update2D() override;
	void Update3D() override;
};