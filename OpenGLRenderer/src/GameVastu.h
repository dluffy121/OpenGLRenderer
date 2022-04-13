#pragma once

#include "Transform.h"
#include <unordered_map>
#include <string>
#include "Component.h"
#include "StaticMethods.h"

class GameVastu
{
public:
	Transform* m_transform;

private:
	unsigned int Id;	// change to assign a proper generated unique Id
	std::unordered_map<std::string, Component*> m_Components; // use shared pointer

public:
	GameVastu();
	GameVastu(const GameVastu& gameVastu);
	~GameVastu();

	inline unsigned int GetId() const { return Id; };
	void SetId(unsigned int id, const std::source_location& location = std::source_location::current());

	template <Component_T T>
	void CreateComponent(T& component)
	{
		const std::string type = GetType<T>();

		if (m_Components.find(type) != m_Components.end())
		{
			Log("Component of type: " << type << " is already added");
			return;
		}

		m_Components[type] = &component;
	}

	template <Component_T T>
	void AddComponent(T& component)
	{
		const std::string type = GetType<T>();

		if (m_Components.find(type) != m_Components.end())
		{
			Log("Component of type: " << type << " is already added");
			return;
		}

		unsigned int id = reinterpret_cast<unsigned int>(&component);
		component.SetId(id);
		component.GameVastuId = Id;

		m_Components[type] = &component;
	}

	template <Component_T T>
	void RemoveComponent(T& component) const
	{
		std::string type = GetType<T>();

		if (m_Components.find(type) == m_Components.end())
			return;

		component.GameVastuId = -1;

		m_Components.erase(type);
	}

	template <Component_T T>
	T* GetComponent() const
	{
		std::string type = GetType<T>();

		//Component& component = &(m_Components[type]);

		//return (dynamic_cast<T&>(component));

		std::unordered_map<std::string, Component*>::const_iterator pos = m_Components.find(type);

		if (pos == m_Components.end())
			return nullptr;

		return dynamic_cast<T*>(pos->second);
	}

	template <Component_T T>
	Component* GetComponent1() const
	{
		std::string type = GetType<T>();

		return m_Components[type];

		//std::unordered_map<std::string, Component*>::const_iterator pos = m_Components.find(type);

		//if (pos == m_Components.end())
		//	return nullptr;

		//return pos->second;
	}
};