#include "Scene.h"
#include <Window/WindowManager.h>
#include <GameVastu/GameVastuManager.h>

scene::Scene::Scene(std::string name) :
	m_Active(true),
	m_Name(name)
{}

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

void scene::Scene::DestroyGameVastu(GameVastu*& gameVastu)
{
	if (!gameVastu)
		return;

	unsigned int id = gameVastu->Id;

	size_t size = m_GameVastus.size();
	size_t i = 0;
	for (; i < size; i++)
		if (m_GameVastus[i]->Id == id)
			break;

	if (i == size)
		return;

	if (WindowManager::getInstance()->SelectedGameVastu() && WindowManager::getInstance()->SelectedGameVastu()->Id == id)
		WindowManager::getInstance()->SelectedGameVastu(NULL);

	m_GameVastus.erase(m_GameVastus.begin() + i);
	GameVastuManager::getInstance()->DestroyGameVastu(gameVastu);
	gameVastu = NULL;
}