#include "Component.h"
#include <string.h>

Component::Component() :
	Id(0)
{
	Awake();
}

Component::~Component()
{
	OnDestroy();
}

void Component::SetId(unsigned int id, const std::source_location& location)
{
	if (!strcmp(location.file_name(), "GameVastu"))
		return;

	Id = id;
}

void Component::_Awake()
{
	unsigned int id = reinterpret_cast<unsigned int>(this);
}

void Component::_Update()
{
	Update();
}

void Component::_OnDestroy()
{
	OnDestroy();
}

void Component::Awake()
{
}

void Component::Update()
{
}

void Component::OnDestroy()
{
}