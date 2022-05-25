#pragma once
#include <string>

template <typename T>
static type_info GetType()
{
	return typeid(T);
}

template <typename T>
static std::string GetTypeName()
{
	return typeid(T).name();
}

template <typename T>
static size_t GetTypeHash()
{
	return typeid(T).hash_code();
}

template<typename T>
struct falseType : std::false_type { };