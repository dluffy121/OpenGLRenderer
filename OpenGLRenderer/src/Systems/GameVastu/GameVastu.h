#pragma once

#include <unordered_map>
#include <string>
#include "Transform.h"
#include <Window/WindowManager.h>
#include "../StaticMethods.h"
#include "../Core/Logger.h"

class GameVastu
{
private:
	friend class GameVastuManager;
	friend class InspectorWindow;

public:
	Transform* m_transform;
	std::string m_name;

private:
	unsigned int Id;	// change to assign a proper generated unique Id
	std::unordered_map<size_t, Component*> m_Components; // use shared pointer

	~GameVastu();

public:
	GameVastu();
	GameVastu(const GameVastu& gameVastu);

	inline unsigned int GetId() const { return Id; };

	template <typename T>
	void CreateComponent(Component& component)
	{
		static_assert(falseType<T>::value);
	}

	template <Component_T T>
	void CreateComponent(T& component)
	{
		size_t type = GetTypeHash<T>();

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
		size_t type = GetTypeHash<T>();

		if (m_Components.find(type) != m_Components.end())
		{
			Log("Component of type: " << type << " is already added");
			return;
		}

		component.gameVastu = this;
		WindowManager::getInstance()->GetCurrentWindow()->RegisterComponent(component);

		m_Components[type] = &component;
	}

	template <Component_T T>
	void RemoveComponent(T& component) const
	{
		size_t type = GetTypeHash<T>();

		if (m_Components.find(type) == m_Components.end())
			return;

		WindowManager::getInstance()->GetCurrentWindow()->UnRegisterComponent(component);

		m_Components.erase(type);
	}

	template <Component_T T>
	T* GetComponent() const
	{
		size_t type = GetTypeHash<T>();

		//Component& component = &(m_Components[type]);
		//return (dynamic_cast<T&>(component));

		std::unordered_map<std::string, Component*>::const_iterator pos = m_Components.find(type);

		if (pos == m_Components.end())
			return nullptr;

		return dynamic_cast<T*>(pos->second);
	}
};