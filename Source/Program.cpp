#include <iostream>
#include <fstream>
#include "raylib-cpp.hpp"
#include "rlgl.h"
#include "PhysicsManager.h"
#include "ControllerComponent.h"
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
    raylib::Camera3D& cam = Scene::GetActive().GetCamera();
    cam.position = { 0.0f, 10.0f, -10.0f };
    cam.target = { 0.0f, 0.0f, 0.0f };
    cam.up = { 0.0f, 1.0f, 0.0f };
    cam.fovy = 45.0f;
    cam.projection = CAMERA_PERSPECTIVE;

    std::string path = "Assets/Scene.json";
    std::ifstream f(path);

    if (!f) { std::cerr << "Error: Failed to open file: " << path << std::endl; }
    else 
    {
        nlohmann::json data = nlohmann::json::parse(f);
        JSONParsing::GenerateSceneFromJSON(data);
    }

    Scene& scene = Scene::GetActive();
    scene.Start();

    SetTargetFPS(144);

    // APPLICATION LOOP //
    while (!window.ShouldClose()) 
    {
        scene.FixedUpdate();

        BeginDrawing();

        window.ClearBackground(raylib::RAYWHITE);

        if (IsKeyPressed(KEY_Q)) { scene.Close(); }

        cam.BeginMode();
        scene.Update3D();
        cam.EndMode();

        scene.Update2D();

        EndDrawing();
    }

    scene.Close();
    physics.Shutdown();

    return 0;
}