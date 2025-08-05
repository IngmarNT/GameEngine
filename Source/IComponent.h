#pragma once
#include <string>
#include <memory>

class GameObject;

class IComponent : public std::enable_shared_from_this<IComponent>
{
protected:
	GameObject& gameObject;
public:
	IComponent(GameObject& gM) : gameObject(gM) {}
	virtual ~IComponent() = default;

	virtual void Initialize() = 0;
	virtual void Start() = 0;
	virtual void Update2D() = 0;
	virtual void Update3D() = 0;
	virtual void FixedUpdate() = 0;
	virtual void Destroy() = 0;

	template<typename T>
	static std::shared_ptr<T> Create(GameObject& gM) 
	{
		static_assert(std::is_base_of<IComponent, T>::value, "T must derive from IComponent");
		auto obj = std::make_shared<T>(gM);
		obj->Initialize();
		return obj;
	}
};