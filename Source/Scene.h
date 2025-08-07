#pragma once
#include <memory>
#include <vector>
#include "GameObject.h"
#include "nlohmann/json.hpp"

class GameObject;

class Scene 
{
private:
	Scene();
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	std::vector<std::shared_ptr<GameObject>> sceneObjects;

	float physicsAlpha = 0.0f;
public:
	static Scene& GetActive();

	bool Open(nlohmann::json data);
	bool Open(std::string path);
	void Start();
	void Update2D();
	void Update3D();
	void FixedUpdate();
	void Close();

	std::shared_ptr<GameObject> FindObject(std::string name);
	bool CheckInScene(std::shared_ptr<GameObject> obj);
	bool AddObject(std::shared_ptr<GameObject> obj);
	bool RemoveObject(std::shared_ptr<GameObject> obj);

	float GetPhysicsAlpha();
};