#pragma once

#include "JoltSetup.h"

class PhysicsManager
{
public:
    static PhysicsManager& Get();

    void Init();
    void Shutdown();

    void Update(float deltaTime);

    PhysicsSystem* GetSystem() { return &mPhysicsSystem; }

private:
    // Allocators and threading
    TempAllocatorImpl* mTempAllocator = nullptr;
    JobSystemThreadPool* mJobSystem = nullptr;

    // Collision filtering and listeners
    BPLayerInterfaceImpl mBroadPhaseLayerInterface;
    ObjectVsBroadPhaseLayerFilterImpl mObjectVsBroadPhaseLayerFilter;
    ObjectLayerPairFilterImpl mObjectLayerPairFilter;
    MyBodyActivationListener mBodyActivationListener;
    MyContactListener mContactListener;

    // The main physics system
    PhysicsSystem mPhysicsSystem;

    PhysicsManager();
    ~PhysicsManager();
};