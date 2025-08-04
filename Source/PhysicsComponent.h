#pragma once
#include "IComponent.h"
#include "DataTypes.h"
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>

class GameObject;

class PhysicsComponent : public IComponent
{
private:
	MyEngine::Vec3 previousPosition;
	Quaternion previousRotation;

	JPH::BodyID bodyID;
	MyEngine::PhysicsBodyConfig bodyConfig;
public:
	PhysicsComponent(GameObject& gM, const MyEngine::PhysicsBodyConfig& config);

	static std::shared_ptr<PhysicsComponent> Create(GameObject& gM, const MyEngine::PhysicsBodyConfig& config);

	void SetBodyID(JPH::BodyID id);
	JPH::BodyID GetBodyID() const;

	void Initialize() override;
	void Start() override;
	void FixedUpdate() override;
	void Update2D() override;
	void Update3D() override;
	
	void SyncToTransform();
	void SyncFromTransform();
};