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

void Component::_OnGUI()
{
	if (!enabled) return;

	OnGUI();
}

void Component::_OnInspectorGUI()
{
	ImGui::PushID(Id);
	ImGui::Checkbox(name.c_str(), &enabled);

	ImGui::Spacing();

	OnInspectorGUI();

	ImGui::PopID();
}

void Component::_OnDestroy()
{
	OnDestroy();
}