#pragma once
#include "nlohmann/json.hpp"
#include "ControllerComponent.h"
#include "PhysicsComponent.h"
#include "ModelComponent.h"
#include <map>
#include <iostream>

namespace JSONParsing
{
	static MyEngine::Vec3 Vec3FromJSON(const std::vector<float>& arr) 
	{
		return MyEngine::Vec3(arr[0], arr[1], arr[2]);
	}

	static JPH::EAllowedDOFs DOFFromJSON(const std::vector<string>& arr) 
	{	
		JPH::EAllowedDOFs dof = JPH::EAllowedDOFs::None;

		for (const auto& str : arr) 
		{
			if (str == "TranslationX") dof |= JPH::EAllowedDOFs::TranslationX;
			else if (str == "TranslationY") dof |= JPH::EAllowedDOFs::TranslationY;
			else if (str == "TranslationZ") dof |= JPH::EAllowedDOFs::TranslationZ;
			else if (str == "Translation3D") dof |= JPH::EAllowedDOFs::TranslationX | JPH::EAllowedDOFs::TranslationY | JPH::EAllowedDOFs::TranslationZ;
			else if (str == "RotationX") dof |= JPH::EAllowedDOFs::RotationX;
			else if (str == "RotationY") dof |= JPH::EAllowedDOFs::RotationY;
			else if (str == "RotationZ") dof |= JPH::EAllowedDOFs::RotationZ;
			else if (str == "All") dof |= JPH::EAllowedDOFs::All;
			else if (str == "Plane2D") dof |= JPH::EAllowedDOFs::Plane2D;
		}
		return dof;
	}

	static JPH::EMotionType MotionTypeFromJSON(const std::string str) 
	{
		if (str == "Dynamic") return JPH::EMotionType::Dynamic;
		else if (str == "Static") return JPH::EMotionType::Static;
		else if (str == "Kinematic") return JPH::EMotionType::Kinematic;

		std::cerr << "Warning: '" << str << "' is not valid motionType, defaulting to Static" << std::endl;
		return JPH::EMotionType::Static;
	}

	static JPH::ObjectLayer ObjectLayerFromJSON(const std::string str) 
	{
		if (str == "MOVING") return Layers::MOVING;
		else if (str == "NON_MOVING") return Layers::NON_MOVING;

		std::cerr << "Warning: '" << str << "' is not valid objectLayer, defaulting to NON_MOVING" << std::endl;
		return Layers::NON_MOVING;
	}

	static MyEngine::PhysicsShapeType PhysicsShapeTypeFromJSON(const std::string str) 
	{
		if (str == "Box") return MyEngine::PhysicsShapeType::Box;
		else if (str == "Sphere") return MyEngine::PhysicsShapeType::Sphere;
		else if (str == "Capsule") return MyEngine::PhysicsShapeType::Capsule;
		else if (str == "Custom") return MyEngine::PhysicsShapeType::Custom;

		std::cerr << "Warning: '" << str << "' is not valid shapeType, defaulting to Box" << std::endl;
		return MyEngine::PhysicsShapeType::Box;
	}

	static MyEngine::DefaultModelShapes ModelShapeFromJSON(const string str)
	{
		if (str == "Box") return MyEngine::DefaultModelShapes::Box;
		else if (str == "Sphere") return MyEngine::DefaultModelShapes::Sphere;
		else if (str == "Torus") return MyEngine::DefaultModelShapes::Torus;
		else if (str == "Cone") return MyEngine::DefaultModelShapes::Cone;

		std::cerr << "Warning: '" << str << "' is not valid modelShape, defaulting to Box" << std::endl;
		return MyEngine::DefaultModelShapes::Box;
	}

	static void GenerateSceneFromJSON(const nlohmann::json sceneData) 
	{
		std::map<std::string, std::shared_ptr<GameObject>> objects;

		for (const auto& objData : sceneData["objects"]) 
		{
			std::cout << "reached [objects]" << std::endl;
			auto obj = GameObject::Create();

			if (objData.contains("name")) 
			{
				std::cout << "reached [name]" << std::endl;
				std::string name = objData["name"];

				if (objects.find(name) != objects.end()) continue;
				else 
				{ 
					obj->SetName(objData["name"]);
					objects.emplace(objData["name"], obj); 
				}
			}

			if (objData.contains("parent")) 
			{
				std::cout << "reached [parent]" << std::endl;
				if (objData["parent"] != nullptr) 
				{
					if (objects.find(objData["parent"]) != objects.end())
					{
						obj->SetParent(objects[objData["parent"]]);
					}
				}
			}

			if (objData.contains("transform")) 
			{
				std::cout << "reached [transform]" << std::endl;
				const auto& t = objData["transform"];
				if (t.contains("position")) obj->GetTransform().position = Vec3FromJSON(t["position"]);
				if (t.contains("rotation")) obj->GetTransform().rotation = MyEngine::Quat::FromEuler(Vec3FromJSON(t["rotation"]));
				if (t.contains("scale"))    obj->GetTransform().scale = Vec3FromJSON(t["scale"]);
			}

			const auto& comps = objData["components"];

			if (comps.contains("Physics")) 
			{
				std::cout << "reached [Physics]" << std::endl;
				const auto& p = comps["Physics"];
				MyEngine::PhysicsBodyConfig config;

				if (p.contains("shape")) 
				{ 
					config.shapeType = PhysicsShapeTypeFromJSON(p["shape"]); 
					if (config.shapeType == MyEngine::PhysicsShapeType::Custom && p.contains("path")) 
					{
						config.meshVertices = MyEngine::GetVerticesFromModel(raylib::Model(p["path"]));
					}
				}
				else config.shapeType = MyEngine::PhysicsShapeType::Box;

				if (p.contains("mass")) config.mass = p["mass"];
				if (p.contains("dof")) config.dof = DOFFromJSON(p["dof"]);
				if (p.contains("motionType")) config.motionType = MotionTypeFromJSON(p["motionType"]);
				if (p.contains("layer")) config.layer = ObjectLayerFromJSON(p["layer"]);
				if (p.contains("scale")) config.scale = Vec3FromJSON(p["scale"]);

				PhysicsComponent::Create(*obj, config);
			}

			if (comps.contains("Model")) 
			{
				std::cout << "reached [Model]" << std::endl;
				if (comps["Model"].contains("path")) 
				{
					std::shared_ptr<raylib::Model> model = make_shared<raylib::Model>(comps["Model"]["path"]);
					ModelComponent::Create(*obj, model, true);
				}
				else if (comps["Model"].contains("shape")) 
				{
					MyEngine::DefaultModelShapes s = ModelShapeFromJSON(comps["Model"]["shape"]);
					ModelComponent::Create(*obj, s, true);
				}
			}

			if (comps.contains("Controller")) 
			{
				IComponent::Create<ControllerComponent>(*obj);
			}
		}
	}
}