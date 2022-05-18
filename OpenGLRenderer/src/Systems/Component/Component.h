#pragma once

#include <type_traits>
#include <source_location>

class GameVastu;

class Component
{
public:
	unsigned int GameVastuId;
	GameVastu* gameVastu;

private:
	unsigned int Id;					// change to assign a proper generated unique Id

public:
	Component();
	~Component();

	inline unsigned int GetId() const { return Id; }
	void SetId(unsigned int id, const std::source_location& location = std::source_location::current());

protected:
	virtual void Awake();
	virtual void Update();
	virtual void OnDestroy();

private:
	void _Awake();
	void _Update();
	void _OnDestroy();
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