#include "GameVastuManager.h"
#include "../../Core/Logger.h"

GameVastuManager::GameVastuManager() {}

GameVastuManager* GameVastuManager::getInstance()
{
	static GameVastuManager instance;
	return &instance;
}

GameVastu* GameVastuManager::CreateGameVastu()
{
	GameVastu* gameVastu = new GameVastu();
	unsigned int id = reinterpret_cast<unsigned int>(gameVastu);
	gameVastu->SetId(id);

	ASSERT(m_GameVastus.find(id) == m_GameVastus.end());

	m_GameVastus[id] = gameVastu;

	return gameVastu;
}

void GameVastuManager::DestroyGameVastu(GameVastu*& gameVastu)
{
	unsigned int id = gameVastu->GetId();
	m_GameVastus.erase(gameVastu->GetId());

	gameVastu->~GameVastu();

	delete gameVastu;			// doubt
	gameVastu = nullptr;
}

GameVastu* GameVastuManager::FindGameVastu(unsigned int id)
{
	return m_GameVastus[id];
}
