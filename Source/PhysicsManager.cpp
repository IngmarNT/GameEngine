#include "PhysicsManager.h"
using namespace JPH;

PhysicsManager::PhysicsManager() = default;

PhysicsManager::~PhysicsManager()
{
    Shutdown();
}

PhysicsManager& PhysicsManager::Get() 
{
    static PhysicsManager instance;
    return instance;
}

void PhysicsManager::Init(float _physicsStep)
{
    // Set trace and assert functions
    Trace = TraceImpl;
#ifdef JPH_ENABLE_ASSERTS
    AssertFailed = AssertFailedImpl;
#endif

    this->physicsStep = _physicsStep;

    // Register all Jolt types
    RegisterDefaultAllocator();
    Factory::sInstance = new Factory();
    RegisterTypes();

    // Create a temporary allocator
    const uint tempAllocatorSize = 10 * 1024 * 1024;
    mTempAllocator = new TempAllocatorImpl(tempAllocatorSize);

    // Create a job system with as many threads as logical cores
    const uint maxJobs = 1024;
    const uint maxBarriers = 64;
    mJobSystem = new JobSystemThreadPool(cMaxPhysicsJobs, cMaxPhysicsBarriers, thread::hardware_concurrency() - 1);

    static BPLayerInterfaceImpl broad_phase_layer_interface;
    static ObjectVsBroadPhaseLayerFilterImpl object_vs_broadphase_layer_filter;
    static ObjectLayerPairFilterImpl object_layer_pair_filter;

    // Initialize physics system
    PhysicsSettings settings;
    mPhysicsSystem.Init(1024, 0, 1024, 1024, broad_phase_layer_interface, object_vs_broadphase_layer_filter, object_layer_pair_filter);

    mPhysicsSystem.SetBodyActivationListener(&mBodyActivationListener);
    mPhysicsSystem.SetContactListener(&mContactListener);
}

void PhysicsManager::Shutdown()
{
    UnregisterTypes();

    delete mJobSystem;
    mJobSystem = nullptr;

    delete mTempAllocator;
    mTempAllocator = nullptr;

    delete Factory::sInstance;
    Factory::sInstance = nullptr;
}

void PhysicsManager::Update(float deltaTime)
{
    constexpr int collisionSteps = 1;

    mPhysicsSystem.Update(deltaTime, collisionSteps, mTempAllocator, mJobSystem);
}