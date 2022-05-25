#include "Scene.h"
#include <Window/WindowManager.h>
#include <GameVastu/GameVastuManager.h>

scene::Scene::Scene(std::string name) :
	m_Active(true),
	m_Name(name)
{
	WindowManager::getInstance()->GetCurrentWindow()->RegisterComponent(*this);
}

scene::Scene::~Scene()
{
	m_Active = false;
}
