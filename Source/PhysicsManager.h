#pragma once

#include "JoltSetup.h"

class PhysicsManager
{
public:
    static PhysicsManager& Get();

    void Init(float _physicsStep = 1.0f / 30.0f);
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
    float physicsStep;

    PhysicsManager();
    ~PhysicsManager();
};