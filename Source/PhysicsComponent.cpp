#include "PhysicsComponent.h"
#include "PhysicsManager.h"
#include "GameObject.h"
#include <Jolt/Geometry/ConvexHullBuilder.h>
#include <Jolt/Physics/Collision/Shape/ConvexHullShape.h>
#include <Jolt/Physics/Collision/Shape/ConvexShape.h>
#include <Jolt/Physics/Constraints/SwingTwistConstraint.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/Factory.h>
#include <vector>

PhysicsComponent::PhysicsComponent(GameObject& gM, const MyEngine::PhysicsBodyConfig& config) : IComponent(gM) 
{
	this->bodyConfig = config;
}

std::shared_ptr<PhysicsComponent> PhysicsComponent::Create(GameObject& gM, const MyEngine::PhysicsBodyConfig& config) 
{
	auto obj = std::make_shared<PhysicsComponent>(gM, config);
	obj->Initialize();
	return obj;
}

void PhysicsComponent::SetBodyID(JPH::BodyID id) { bodyID = id; }

JPH::BodyID PhysicsComponent::GetBodyID() const { return bodyID; }

void PhysicsComponent::Initialize() 
{
	gameObject.AddComponent(shared_from_this());
}

void PhysicsComponent::Start() 
{
	JPH::RefConst<JPH::Shape> shape;

	switch (bodyConfig.shapeType) 
	{
	case MyEngine::PhysicsShapeType::Box:
		shape = new JPH::BoxShape(bodyConfig.dimensions);
		break;
	case MyEngine::PhysicsShapeType::Sphere:
		shape = new JPH::SphereShape(bodyConfig.radius);
		break;
	case MyEngine::PhysicsShapeType::Capsule:
		shape = new JPH::CapsuleShape(bodyConfig.radius, bodyConfig.height);
		break;
	case MyEngine::PhysicsShapeType::Custom: {
		JPH::Array<JPH::Vec3> pointCloud;
		pointCloud.reserve(bodyConfig.meshVertices.size());

		for (const auto& v : bodyConfig.meshVertices)
		{
			pointCloud.push_back(JPH::Vec3(v.x, v.y, v.z));
		}

		JPH::ConvexHullShapeSettings settings(pointCloud);
		settings.mMaxConvexRadius = 0.05f;

		JPH::ShapeSettings::ShapeResult result = settings.Create();
		if (result.HasError())
		{
			std::cerr << "Convex hull shape creation failed: " << result.GetError() << std::endl;
		}

		shape = result.Get();

		break;
		}
	}

	auto& transform = gameObject.GetTransform();

	Quaternion q = transform.rotation.Get();

	JPH::BodyCreationSettings settings(
		shape,
		JPH::RVec3(transform.position.x, transform.position.y, transform.position.z),
		JPH::Quat(q.x, q.y, q.z, q.w),
		bodyConfig.motionType,
		bodyConfig.layer
	);
	settings.mMotionQuality = JPH::EMotionQuality::Discrete;
	settings.mOverrideMassProperties = JPH::EOverrideMassProperties::CalculateInertia;
	settings.mMassPropertiesOverride.mMass = bodyConfig.mass;
	settings.mFriction = bodyConfig.friction;
	settings.mRestitution = bodyConfig.restitution;
	settings.mUserData = bodyConfig.userData;
	settings.mAllowedDOFs = bodyConfig.dof;

	JPH::PhysicsSystem& system = PhysicsManager::Get().GetSystem();
	JPH::BodyInterface& bodyInterface = system.GetBodyInterface();

	bodyID = bodyInterface.CreateAndAddBody(settings, bodyConfig.activationType);
}

void PhysicsComponent::FixedUpdate() {}

void PhysicsComponent::SyncToTransform() 
{
    auto& transform = gameObject.GetTransform();
    PhysicsManager& pM = PhysicsManager::Get();
    JPH::PhysicsSystem& physics = pM.GetSystem();
    auto* bodyInterface = &physics.GetBodyInterface();

    bodyInterface->SetPosition(bodyID, JPH::RVec3(transform.position.x, transform.position.y, transform.position.z), JPH::EActivation::Activate);
    Quaternion q = transform.rotation.Get();
    bodyInterface->SetRotation(bodyID, JPH::Quat(q.x, q.y, q.z, q.w), JPH::EActivation::Activate);
}

void PhysicsComponent::SyncFromTransform() 
{
    auto& transform = gameObject.GetTransform();
    JPH::PhysicsSystem& physics = PhysicsManager::Get().GetSystem();
    auto* bodyInterface = &physics.GetBodyInterface();

    JPH::RVec3 pos = bodyInterface->GetPosition(bodyID);
	JPH::Quat rot = bodyInterface->GetRotation(bodyID);
    transform.position = MyEngine::Vec3(pos.GetX(), pos.GetY(), pos.GetZ());
	transform.rotation = MyEngine::Quat(rot.GetX(), rot.GetY(), rot.GetZ(), rot.GetW());
}

void PhysicsComponent::Update2D() {}
void PhysicsComponent::Update3D() {}