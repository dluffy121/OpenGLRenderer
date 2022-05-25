#pragma once

#include <unordered_map>

#include <GameVastu/GameVastu.h>

class GameVastuManager
{
private:
	std::unordered_map<unsigned int, GameVastu*> m_GameVastus;

public:
	static GameVastuManager* getInstance();

	GameVastu* CreateGameVastu();
	void DestroyGameVastu(GameVastu*& gameVastu);

	GameVastu* FindGameVastu(unsigned int id);

protected:
	GameVastuManager();

private:
	GameVastuManager(const GameVastuManager&) = delete;
	GameVastuManager operator=(const GameVastuManager&) = delete;
};