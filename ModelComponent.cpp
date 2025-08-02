#include "ModelComponent.h"
#include "GameObject.h"
#include <iostream>

ModelComponent::ModelComponent(GameObject& gM, std::shared_ptr<raylib::Model> _model) : model(_model), IComponent(gM) 
{
	std::cout << model.get() << endl;
}

std::shared_ptr<ModelComponent> ModelComponent::Create(GameObject& gM, std::shared_ptr<raylib::Model> _model)
{
	auto obj = std::make_shared<ModelComponent>(gM, _model);
	obj->Initialize();
	return obj;
}

std::shared_ptr<ModelComponent> ModelComponent::Create(GameObject& gM, MyEngine::DefaultShapes shape)
{
	auto m = make_shared<raylib::Model>();
	MyEngine::GameTransform& t = gM.GetTransform();

	switch (shape) 
	{
	case(MyEngine::DefaultShapes::Box):
		m->Load(GenMeshCube(1.0f, 1.0f, 1.0f));
		break;
	case(MyEngine::DefaultShapes::Sphere):
		m->Load(GenMeshSphere(1.0f, 8, 16));
		break;
	case(MyEngine::DefaultShapes::Torus):
		m->Load(GenMeshTorus(1.0f, 1.0f, 8, 16));
		break;
	case(MyEngine::DefaultShapes::Cone):
		m->Load(GenMeshCone(1.0f, 1.0f, 8));
		break;
	}

	return Create(gM, m);
}

void ModelComponent::Initialize() 
{
	gameObject.AddComponent(shared_from_this());
}
void ModelComponent::Start() {}
void ModelComponent::FixedUpdate() {}
void ModelComponent::Update2D() {}
void ModelComponent::Update3D() 
{
	std::cout << "..." << std::endl;

	MyEngine::GameTransform& transform = gameObject.GetTransform();

	raylib::Quaternion _q = transform.rotation.Get();

	JPH::Quat q(_q.x, _q.y, _q.z, _q.w);
	JPH::Mat44 mat = JPH::Mat44::sRotation(q);

	rlPushMatrix(); // Save current matrix
		rlTranslatef(transform.position.x, transform.position.y, transform.position.z);
		rlScalef(transform.scale.x * 2.0f, transform.scale.y * 2.0f, transform.scale.z * 2.0f);
		rlMultMatrixf((float*)&mat);
		DrawModel(*model, Vector3(), 1.0f, raylib::RED);
		DrawModelWires(*model, Vector3(), 1.0f, raylib::DARKGRAY);
	rlPopMatrix(); // Restore saved matrix
}