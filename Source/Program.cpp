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
    std::ifstream f(path);

    Scene& scene = Scene::GetActive();

    if (!f) { std::cerr << "Error: Failed to open file: " << path << std::endl; }
    else 
    {
        nlohmann::json data = nlohmann::json::parse(f);
        f.close();

        if (scene.Open(data)) scene.Start();
        else { std::cerr << "Error: " << path << " could not be opened as a Scene" << std::endl; }
    }

    SetTargetFPS(144);

    // APPLICATION LOOP //
    while (!window.ShouldClose()) 
    {
        static CameraComponent* camComp = CameraComponent::GetActive();

        scene.FixedUpdate();

        BeginDrawing();

        window.ClearBackground(raylib::RAYWHITE);

        if (camComp != nullptr) {
            raylib::Camera& cam = camComp->GetCamera();

            cam.BeginMode();
            scene.Update3D();
            cam.EndMode();

            scene.Update2D();
        }

        EndDrawing();
    }

    scene.Close();
    physics.Shutdown();

    return 0;
}