#pragma once
#include <string>

template <typename T>
static std::string GetType()
{
	return typeid(T).name();
}