#include "CameraManager.h"

CameraManager::CameraManager()
{}

CameraManager::~CameraManager()
{}

void CameraManager::Subscribe(Camera* camera)
{
	m_Cameras.push_back(camera);
}

void CameraManager::UnSubscribe(Camera* camera)
{
	for (auto i = m_Cameras.begin(); i != m_Cameras.end(); i++)
	{
		if (*i == camera)
		{
			m_Cameras.erase(i);
			return;
		}
	}
}

Camera* CameraManager::GetCamera()
{
	auto i = m_Cameras.end();
	while (i != m_Cameras.begin())
	{
		--i;

		if (!(*i))
		{
			m_Cameras.erase(i);
			continue;
		}

		if ((*i)->GetEnabled())
			return *i;
	}
}