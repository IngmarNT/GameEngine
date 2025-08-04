#include <iostream>
#include <cassert>
#include "raylib-cpp.hpp"
#include "rlgl.h"
#include "PhysicsManager.h"
#include "ExampleComponent.h"
#include "PhysicsComponent.h"
#include "ModelComponent.h"
#include "Scene.h"

#include "nlohmann/json.hpp"
#include <Jolt.h>

int main()
{
    // PHYSICS SETUP //
    float physicsStep = 1.0f / 30.0f;

    PhysicsManager& physics = PhysicsManager::Get();
    physics.Init(physicsStep);

    // SCENE SETUP //
    int screenWidth = 1280;
    int screenHeight = 720;

    raylib::Window window(screenWidth, screenHeight, "engine-cpp");
    raylib::Camera3D& cam = Scene::GetActive().GetCamera();
    cam.position = { 0.0f, 10.0f, -10.0f };
    cam.target = { 0.0f, 0.0f, 0.0f };
    cam.up = { 0.0f, 1.0f, 0.0f };
    cam.fovy = 45.0f;
    cam.projection = CAMERA_PERSPECTIVE;

    auto obj1 = GameObject::Create();
    auto obj2 = GameObject::Create(obj1);

    std::shared_ptr<raylib::Model> monkey = make_shared<raylib::Model>("Assets/Monkey.obj");

    MyEngine::PhysicsBodyConfig config1;
    config1.shapeType = MyEngine::PhysicsShapeType::Custom;
    config1.meshVertices = MyEngine::GetVerticesFromModel(*monkey);
    config1.mass = 1.0f;
    config1.dof = JPH::EAllowedDOFs::All; //JPH::EAllowedDOFs::TranslationX | JPH::EAllowedDOFs::TranslationY | JPH::EAllowedDOFs::TranslationZ;
    
    MyEngine::PhysicsBodyConfig config2;
    config2.shapeType = MyEngine::PhysicsShapeType::Box;
    config2.motionType = JPH::EMotionType::Static;
    config2.layer = Layers::NON_MOVING;
    config2.dimensions = JPH::Vec3(5.0f, 1.0f, 5.0f);

    obj1->GetTransform().Translate(MyEngine::Vec3(0.0f, 3.0f, 0.0f));
    obj2->GetTransform().Translate(MyEngine::Vec3(0.0f, -1.01f, 0.0f));
    obj1->GetTransform().Scale(MyEngine::Vec3(0.5f, 0.5f, 0.5f));
    obj2->GetTransform().Scale(MyEngine::Vec3(5.0f, 1.0f, 5.0f));
    obj1->GetTransform().Rotate(MyEngine::Quat::FromEuler(0.0f, 180.0f, 0.0f));

    (void)IComponent::Create<ExampleComponent>(*obj1);

    (void)ModelComponent::Create(*obj1, monkey, true);
    (void)ModelComponent::Create(*obj2, MyEngine::DefaultShapes::Box);

    auto p1 = PhysicsComponent::Create(*obj1, config1);
    (void)PhysicsComponent::Create(*obj2, config2);

    Scene& scene = Scene::GetActive();
    scene.Start();

    SetTargetFPS(144);

    // APPLICATION LOOP //
    while (!window.ShouldClose()) 
    {
        scene.FixedUpdate();

        BeginDrawing();

        window.ClearBackground(raylib::RAYWHITE);

        cam.BeginMode();

            scene.Update3D();

            //DrawGrid(10, 1.0f);
            
        cam.EndMode();

        scene.Update2D();

        EndDrawing();
    }

    return 0;
}