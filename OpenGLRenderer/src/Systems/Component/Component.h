#pragma once

#include <type_traits>
#include <source_location>
#include "glm/glm.hpp"
#include "../../Core/GL/VertexArray/VertexArray.h"

class GameVastu;

class Component
{
private:
	friend class GameVastu;

public:
	bool m_Enabled;

protected:
	GameVastu* gameVastu;

private:
	unsigned int Id;					// change to assign a proper generated unique Id

public:
	Component();
	virtual ~Component();

	inline unsigned int GetId() const { return Id; }
	inline GameVastu* GetGameVastu() { return gameVastu; }

	virtual void Awake(VertexArray* va);
	virtual void Update();
	virtual void Render(const glm::mat4 vp);
	virtual void OnGUI();
	virtual void OnDestroy();
};

template<class T>
concept Component_T = requires
{
	std::is_base_of<Component, T>::value == true && std::is_pointer<T>::value == true;
};

template<class T>
concept Component_T_ptr = requires
{
	std::is_base_of<Component*, T*>::value == true && std::is_pointer<T*>::value == true;
};