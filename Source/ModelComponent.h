#pragma once
#include "IComponent.h"
#include "raylib-cpp.hpp"
#include "MyEngineTypes.h"

class GameObject;

class ModelComponent : public IComponent 
{
private:
	std::shared_ptr<raylib::Model> model;
	bool doInterpolation;
public:
	ModelComponent(GameObject& gM, std::shared_ptr<raylib::Model> _model, bool interp);

	static std::shared_ptr<ModelComponent> Create(GameObject& gM, MyEngine::DefaultModelShapes shape, bool interp = false);
	static std::shared_ptr<ModelComponent> Create(GameObject& gM, std::shared_ptr<raylib::Model> _model, bool interp = false);

	void Initialize() override;
	void Start() override;
	void FixedUpdate() override;
	void Update2D() override;
	void Update3D() override;
	void Destroy() override;
};