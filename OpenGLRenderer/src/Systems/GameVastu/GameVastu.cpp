#include "GameVastu.h"
#include <type_traits>

GameVastu::GameVastu() :
	m_transform(new Transform())
{
#pragma warning (disable : 4311 4302)
	Id = reinterpret_cast<unsigned int>(this);
#pragma warning (default : 4311 4302)
}

GameVastu::GameVastu(const GameVastu& gameVastu) :
	Id(gameVastu.Id),
	m_transform(gameVastu.m_transform)
{}

GameVastu::~GameVastu()
{}