#include "GameVastu.h"
#include <type_traits>

GameVastu::GameVastu() :
	m_transform(new Transform())
{}

GameVastu::GameVastu(const GameVastu& gameVastu) :
	Id(gameVastu.Id),
	m_transform(gameVastu.m_transform)
{}

GameVastu::~GameVastu()
{}

void GameVastu::SetId(unsigned int id, const std::source_location& location)
{
	if (!strcmp(location.file_name(), "GameVastuManager"))
		return;

	Id = id;
}