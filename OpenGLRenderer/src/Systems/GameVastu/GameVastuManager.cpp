#include "GameVastuManager.h"
#include <Logger/Logger.h>

GameVastuManager::GameVastuManager() {}

GameVastuManager* GameVastuManager::getInstance()
{
	static GameVastuManager instance;
	return &instance;
}

GameVastu* GameVastuManager::CreateGameVastu()
{
	GameVastu* gameVastu = new GameVastu();
	unsigned int id = gameVastu->Id;

	ASSERT(m_GameVastus.find(id) == m_GameVastus.end());

	m_GameVastus[id] = gameVastu;

	return gameVastu;
}

void GameVastuManager::DestroyGameVastu(GameVastu*& gameVastu)
{
	m_GameVastus.erase(gameVastu->Id);

	delete gameVastu;
}

GameVastu* GameVastuManager::FindGameVastu(unsigned int id)
{
	return m_GameVastus[id];
}
