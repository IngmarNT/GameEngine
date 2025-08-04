#include "ModelComponent.h"
#include "GameObject.h"
#include "DataTypes.h"

ModelComponent::ModelComponent(GameObject& gM, std::shared_ptr<raylib::Model> _model, bool interp) : 
	doInterpolation(interp), model(_model), IComponent(gM) {}

std::shared_ptr<ModelComponent> ModelComponent::Create(GameObject& gM, std::shared_ptr<raylib::Model> _model, bool interp)
{
	auto obj = std::make_shared<ModelComponent>(gM, _model, interp);
	obj->Initialize();
	return obj;
}

std::shared_ptr<ModelComponent> ModelComponent::Create(GameObject& gM, MyEngine::DefaultShapes shape, bool interp)
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

	return Create(gM, m, interp);
}

void ModelComponent::Initialize() 
{
	gameObject.AddComponent(shared_from_this());
}

void ModelComponent::Update3D() 
{
	float alpha = Scene::GetActive().GetPhysicsAlpha();
	MyEngine::GameTransform& transform = gameObject.GetTransform();

	MyEngine::Vec3 pos = doInterpolation ? transform.GetInterpolatedPosition(alpha) : transform.position;
	Quaternion rot = doInterpolation ? transform.GetInterpolatedRotation(alpha) : transform.rotation.Get();

	rlPushMatrix();
		rlTranslatef(pos.x, pos.y, pos.z);
		rlScalef(transform.scale.x * 2.0f, transform.scale.y * 2.0f, transform.scale.z * 2.0f);

		JPH::Quat q(rot.x, rot.y, rot.z, rot.w);
		JPH::Mat44 mat = JPH::Mat44::sRotation(q);
		rlMultMatrixf((float*)&mat);

		DrawModel(*model, Vector3(), 1.0f, raylib::RED);
		DrawModelWires(*model, Vector3(), 1.0f, raylib::DARKGRAY);
	rlPopMatrix();
}

void ModelComponent::Start() {}
void ModelComponent::FixedUpdate() {}
void ModelComponent::Update2D() {}