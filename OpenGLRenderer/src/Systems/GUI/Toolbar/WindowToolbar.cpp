#include "WindowToolbar.h"
#include <Window/Window.h>

WindowToolbar::WindowToolbar() :
	GUIWindow("WindowToolbar")
{}

WindowToolbar::~WindowToolbar()
{}

void WindowToolbar::Draw(Window* window)
{
	ImGui::Begin("WindowToolbar");

	if (!window->IsFullScreen())
	{
		if (ImGui::Button("Fullscreen"))
			window->EnableFullScreen();
	}
	else
	{
		if (ImGui::Button("Windowed"))
			window->EnableWindowed();
	}

	ImGui::End();
}