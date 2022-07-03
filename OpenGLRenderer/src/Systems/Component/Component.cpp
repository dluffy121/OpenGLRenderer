#include "Component.h"
#include <GameVastu/GameVastu.h>
#include <Window/WindowManager.h>
#include <string.h>

Component::Component() :
	Id(reinterpret_cast<unsigned int>(this)),
	gameVastu(nullptr),
	enabled(false),
	name("Component")
{}

Component::~Component()
{
	WindowManager::getInstance()->GetCurrentWindow()->UnRegisterComponent(*this);
}

void Component::SetEnabled(bool value)
{
	if (enabled == value) return;

	enabled = value;
	enabled ? _OnEnable() : _OnDisable();
}

void Component::_Awake()
{
	Awake();
}

void Component::_Update()
{
	if (!enabled) return;

	Update();
}

void Component::_Render()
{
	if (!enabled) return;

	Render();
}

void Component::_OnEnable()
{
	OnEnable();
}

void Component::_OnDisable()
{
	OnDisable();
}

void Component::_OnGUI()
{
	if (!enabled) return;

	OnGUI();
}

void Component::_OnInspectorGUI()
{
	ImGui::PushID(Id);

	bool value = enabled;
	if (ImGui::Checkbox(name.c_str(), &value))
		SetEnabled(value);

	ImGui::Spacing();

	OnInspectorGUI();

	ImGui::PopID();
}

void Component::_OnDestroy()
{
	OnDestroy();
}