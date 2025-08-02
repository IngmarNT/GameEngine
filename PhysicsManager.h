#pragma once

#include "JoltSetup.h"

class PhysicsManager
{
public:
    static PhysicsManager& Get();

    void Init();
    void Shutdown();

    void Update(float deltaTime);

    JPH::PhysicsSystem& GetSystem() { return mPhysicsSystem; }

private:
    // Allocators and threading
    JPH::TempAllocatorImpl* mTempAllocator = nullptr;
    JPH::JobSystemThreadPool* mJobSystem = nullptr;

    // Collision filtering and listeners
    BPLayerInterfaceImpl mBroadPhaseLayerInterface;
    ObjectVsBroadPhaseLayerFilterImpl mObjectVsBroadPhaseLayerFilter;
    ObjectLayerPairFilterImpl mObjectLayerPairFilter;
    MyBodyActivationListener mBodyActivationListener;
    MyContactListener mContactListener;

    // The main physics system
    JPH::PhysicsSystem mPhysicsSystem;

    PhysicsManager();
    ~PhysicsManager();
};