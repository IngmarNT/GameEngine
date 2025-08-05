#pragma once
#include "raylib-cpp.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "IComponent.h"
#include "PhysicsComponent.h"
#include "Scene.h"
#include "MyEngineTypes.h"

class IComponent; //Forward declaration to avoid circular dependancy
class PhysicsComponent;
class Scene;

class GameObject : public std::enable_shared_from_this<GameObject>
{
private:
	std::string name;
	MyEngine::GameTransform transform;
	std::weak_ptr<GameObject> parent;
	std::vector<std::shared_ptr<GameObject>> children;
	std::vector<std::shared_ptr<IComponent>> components;

	std::weak_ptr<PhysicsComponent> cachedPhysics;

	bool AddChild(std::shared_ptr<GameObject> child);
	bool RemoveChild(std::shared_ptr<GameObject> child);
public:
	static std::shared_ptr<GameObject> Create(std::shared_ptr<GameObject> _parent = nullptr, bool addToScene = true);
	GameObject();

	void Start();
	void Update2D();
	void Update3D();
	void FixedUpdate();

	std::string GetName();
	MyEngine::GameTransform& GetTransform();
	std::weak_ptr<GameObject> GetParent();
	
	void SetName(std::string _name);
	bool SetParent(std::shared_ptr<GameObject> newParent);
	bool RemoveParent();

	bool AddComponent(std::shared_ptr<IComponent> component);
	bool RemoveComponent(std::shared_ptr<IComponent> component);

	void Destroy(bool destroyTree = false);

	template<typename T>
	std::shared_ptr<T> GetComponent() 
	{
		if constexpr (std::is_same<T, PhysicsComponent>::value) {

			if (auto cached = cachedPhysics.lock()) 
			{ 
				return cached;
			}
		}

		for (auto& c : components) {
			if (auto casted = std::dynamic_pointer_cast<T>(c)) {
				if constexpr (std::is_same<T, PhysicsComponent>::value) {
					cachedPhysics = casted;
				}
				return casted;
			}
		}

		return nullptr;
	}
};