#pragma once
#include "IComponent.h"
#include "raylib-cpp.hpp"
#include "DataTypes.h"

class GameObject;

class ModelComponent : public IComponent 
{
private:
	std::shared_ptr<raylib::Model> model;
public:
	ModelComponent(GameObject& gM, std::shared_ptr<raylib::Model> _model);

	static std::shared_ptr<ModelComponent> Create(GameObject& gM, MyEngine::DefaultShapes shape);
	static std::shared_ptr<ModelComponent> Create(GameObject& gM, std::shared_ptr<raylib::Model> _model);

	void Initialize() override;
	void Start() override;
	void FixedUpdate() override;
	void Update2D() override;
	void Update3D() override;
};