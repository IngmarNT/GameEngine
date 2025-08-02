#pragma once
#include <memory>
#include <vector>
#include "GameObject.h"

class GameObject;

class Scene 
{
private:
	Scene();
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	std::vector<std::shared_ptr<GameObject>> sceneObjects;
	raylib::Camera3D mainCamera;
public:
	static Scene& GetActive();
	raylib::Camera3D& GetCamera();

	void Start();
	void Update2D();
	void Update3D();
	void FixedUpdate();

	bool CheckInScene(std::shared_ptr<GameObject> obj);
	bool AddObject(std::shared_ptr<GameObject> obj);
	bool RemoveObject(std::shared_ptr<GameObject> obj);
};