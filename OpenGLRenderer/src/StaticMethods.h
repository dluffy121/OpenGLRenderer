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
T* CopyArray(T*& arrayToCopy, unsigned int size)
{
	T* array = new T[size];
	for (size_t i = 0; i < size; i++)
	{
		array[i] = arrayToCopy[i];
	}
	return array;
}