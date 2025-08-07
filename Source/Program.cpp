#include <iostream>
#include <fstream>
#include "raylib-cpp.hpp"
#include "rlgl.h"
#include "PhysicsManager.h"
#include "ControllerComponent.h"
#include "CameraComponent.h"
#include "PhysicsComponent.h"
#include "ModelComponent.h"
#include "Scene.h"
#include "JSONParsing.h"

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

    std::string path = "Assets/Scene.json";

    Scene& scene = Scene::GetActive();

    bool sceneOpen = scene.Open(path);

    SetTargetFPS(144);

    CameraComponent* camComp = CameraComponent::GetActive();

    // APPLICATION LOOP //
    while (!window.ShouldClose()) 
    {
        scene.FixedUpdate();

        BeginDrawing();

        window.ClearBackground(raylib::RAYWHITE);

        if (IsKeyPressed(KEY_Q) && sceneOpen == true) 
        {
            std::cout << "Closing scene..." << std::endl;
            scene.Close();
            sceneOpen = false;
            camComp = nullptr;
        }

        if (IsKeyPressed(KEY_E) && sceneOpen == false) 
        {
            std::cout << "Opening scene from: " << path << std::endl;
            sceneOpen = scene.Open(path);

            camComp = CameraComponent::GetActive();
        }

        if (camComp != nullptr) {
            raylib::Camera& cam = camComp->GetCamera();

            cam.BeginMode();
            scene.Update3D();
            cam.EndMode();

            scene.Update2D();
        }

        DrawFPS(10, 10);

        EndDrawing();
    }

    camComp = nullptr;

    scene.Close();
    physics.Shutdown();

    return 0;
}