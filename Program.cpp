#include <iostream>
#include <cassert>
#include "raylib-cpp.hpp"
#include "GameObject.h"
#include "ExampleComponent.h"`
#include "Scene.h"

#include <Jolt.h>

int main()
{
    // SCENE SETUP //
    int screenWidth = 1000;
    int screenHeight = 700;

    raylib::Window window(screenWidth, screenHeight, "engine-cpp");
    raylib::Camera3D& cam = Scene::GetActive().GetCamera();
    cam.position = { 0.0f, 10.0f, -10.0f };
    cam.target = { 0.0f, 0.0f, 0.0f };
    cam.up = { 0.0f, 1.0f, 0.0f };
    cam.fovy = 45.0f;
    cam.projection = CAMERA_PERSPECTIVE;

    auto obj1 = GameObject::Create();
    auto obj2 = GameObject::Create(obj1);
    obj1->SetParent(obj2);

    (void)IComponent::Create<ExampleComponent>(*obj1);

    Scene::GetActive().Start();

    SetTargetFPS(60);

    // APPLICATION LOOP //
    while (!window.ShouldClose()) 
    {
        BeginDrawing();

        window.ClearBackground(RAYWHITE);

        cam.BeginMode();

            Scene::GetActive().Update3D();

            Vec3 pos = obj1->GetTransform().position;

            DrawCube(pos, 2, 2, 2, RED);
            DrawCubeWires(pos, 2, 2, 2, BLACK);

            DrawGrid(10, 1.0f);
            
        cam.EndMode();

        Scene::GetActive().Update2D();

        EndDrawing();
    }

    return 0;
}