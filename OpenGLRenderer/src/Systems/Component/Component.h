#pragma once

#include <type_traits>
#include <string>

class GameVastu;

class Component
{
private:
	friend class GameVastu;

public:
	bool enabled;

protected:
	GameVastu* gameVastu;
	std::string name;

private:
	const unsigned int Id;					// change to assign a proper generated unique Id

public:
	Component();
	virtual ~Component();

	inline const std::string& GetName() const { return name; }
	inline unsigned int GetId() const { return Id; }
	inline GameVastu* GetGameVastu() { return gameVastu; }

	void _Awake();
	void _Update();
	void _Render();
	void _OnGUI();
	void _OnInspectorGUI();
	void _OnDestroy();

protected:
	virtual void Awake() {}
	virtual void Update() {}
	virtual void Render() {}
	virtual void OnGUI() {}
	virtual void OnInspectorGUI() {}
	virtual void OnDestroy() {}
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