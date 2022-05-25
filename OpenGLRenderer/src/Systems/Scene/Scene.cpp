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

GameVastu* scene::Scene::CreateGameVastu()
{
	GameVastu* gameVastu = GameVastuManager::getInstance()->CreateGameVastu();
	m_GameVastus.push_back(gameVastu);
	return gameVastu;
}

void scene::Scene::DestroyGameVastu(GameVastu* gameVastu)
{
	size_t size = m_GameVastus.size();
	size_t i = 0;
	for (; i < size; i++)
		if (m_GameVastus[i]->GetId() == gameVastu->GetId())		
			break;

	if (i == size)
		return;

	m_GameVastus.erase(m_GameVastus.begin() - i);
	GameVastuManager::getInstance()->DestroyGameVastu(gameVastu);
}