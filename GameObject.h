#pragma once
#include "raylib-cpp.hpp"
#include <string>
#include <vector>
#include <memory>
#include "IComponent.h"
#include "Scene.h"
#include "RaylibClassWrappers.h"
using namespace MyEngine;

class IComponent; //Forward declaration to avoid circular dependancy
class Scene;

class GameObject : public std::enable_shared_from_this<GameObject>
{
private:
	std::string name;
	GameTransform transform;
	std::weak_ptr<GameObject> parent;
	std::vector<std::shared_ptr<GameObject>> children;
	std::vector<std::shared_ptr<IComponent>> components;

	bool AddChild(std::shared_ptr<GameObject> child);
	bool RemoveChild(std::shared_ptr<GameObject> child);
public:
	static std::shared_ptr<GameObject> Create(std::shared_ptr<GameObject> _parent = nullptr, bool addToScene = true);
	GameObject();

	void Start(); //before loop
	void Update2D(); //before 3d camera mode in draw
	void Update3D(); //every frame of game

	std::string GetName();
	GameTransform& GetTransform();
	std::weak_ptr<GameObject> GetParent();
	
	void SetName(std::string _name);
	bool SetParent(std::shared_ptr<GameObject> newParent);
	bool RemoveParent();

	bool AddComponent(std::shared_ptr<IComponent> component);
	bool RemoveComponent(std::shared_ptr<IComponent> component);

	void Destroy(bool destroyTree = false);
};