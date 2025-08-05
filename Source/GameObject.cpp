#include "GameObject.h"
using namespace MyEngine;

GameObject::GameObject()
{
	this->name = "Game Object";

	transform.position = Vec3(0, 0, 0);
	transform.rotation = Quat::Identity();
	transform.scale = Vec3(1, 1, 1);
}

std::shared_ptr<GameObject> GameObject::Create(std::shared_ptr<GameObject> _parent, bool addToScene) 
{
	auto obj = std::make_shared<GameObject>();
	if (_parent) obj->SetParent(_parent);
	if (addToScene) Scene::GetActive().AddObject(obj);
	return obj;
}

void GameObject::Start() 
{
	for (auto component : this->components)
	{
		component->Start();
	}
}

void GameObject::Update2D() 
{
	for (auto component : this->components) 
	{
		component->Update2D();
	}
}

void GameObject::Update3D() 
{
	for (auto component : this->components)
	{
		component->Update3D();
	}
}

void GameObject::FixedUpdate()
{
	for (auto component : this->components)
	{
		component->FixedUpdate();
	}
}

std::string GameObject::GetName() { return this->name; }

GameTransform& GameObject::GetTransform() { return this->transform; }

std::weak_ptr<GameObject> GameObject::GetParent() { return this->parent; }

void GameObject::SetName(std::string _name) { this->name = _name; }

bool GameObject::SetParent(std::shared_ptr<GameObject> newParent)
{
	if (newParent.get() == this) return false;

	if (std::shared_ptr<GameObject> p = parent.lock()) 
	{ 
		if (p.get() == newParent.get()) return true;
		p->RemoveChild(shared_from_this()); 
	}

	this->parent = newParent;
	newParent->AddChild(shared_from_this());

	return true;
}


bool GameObject::RemoveParent() 
{
	if (auto p = parent.lock()) {

		bool removed = p->RemoveChild(shared_from_this());

		parent.reset();

		return removed;
	}
	else return false;
}

bool GameObject::AddChild(std::shared_ptr<GameObject> child) 
{
	for (auto c : this->children) 
	{
		if (c.get() == child.get()) return false;
	}

	children.push_back(child);
	return true;
}

bool GameObject::RemoveChild(std::shared_ptr<GameObject> child)
{
	if (this->children.empty()) return false;

	for (auto it = this->children.begin(); it != this->children.end(); ++it) 
	{
		if ((*it) == child) 
		{
			children.erase(it);
			return true;
		}
	}

	return false;
}

bool GameObject::AddComponent(std::shared_ptr<IComponent> component) 
{
	for (auto c : this->components)
	{
		if (c.get() == component.get()) return false;
	}

	components.push_back(component);
	return true;
}

bool GameObject::RemoveComponent(std::shared_ptr<IComponent> component)
{
	if (this->components.empty()) return false;

	for (auto it = this->components.begin(); it != this->components.end(); ++it)
	{
		if ((*it) == component)
		{
			components.erase(it);
			return true;
		}
	}

	return false;
}

void GameObject::Destroy(bool destroyTree) 
{
	for (auto c : children) 
	{
		destroyTree ? c->Destroy(true) : (void)c->RemoveParent();
	}

	for (size_t i = components.size(); i > 0; i--) 
	{
		components[0]->Destroy();
	}

	this->RemoveParent();
	Scene::GetActive().RemoveObject(shared_from_this());
}