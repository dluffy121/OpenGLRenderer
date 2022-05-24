#include "Component.h"
#include <string.h>
#include "../Window/WindowManager.h"

Component::Component() :
	gameVastu(nullptr)
{
	Id = reinterpret_cast<unsigned int>(this);
}

Component::~Component()
{
	WindowManager::getInstance()->GetCurrentWindow()->UnRegisterComponent(*this);
}

void Component::Awake()
{
}

void Component::Update()
{
	if (!m_Enabled) return;
}

void Component::Render(const glm::mat4 vp)
{
	if (!m_Enabled) return;
}

void Component::OnGUI()
{
	if (!m_Enabled) return;
}

void Component::OnDestroy()
{
}