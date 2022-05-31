#include "GameVastu.h"
#include <type_traits>

GameVastu::GameVastu() :
	m_transform(new Transform()),
	Id(reinterpret_cast<unsigned int>(this))
{}

GameVastu::GameVastu(const GameVastu& gameVastu) :
	Id(gameVastu.Id),
	m_transform(gameVastu.m_transform)
{}

GameVastu::~GameVastu()
{}