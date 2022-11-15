#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

namespace core::gl
{
	static const unsigned long long GLUINT_SIZE = sizeof(GLuint);
	static const unsigned long long GLUSHORT_SIZE = sizeof(GLushort);

	extern unsigned int GetSizeOfType(unsigned int type);

	class OpenGLHelper
	{
	public:
		static int InitializeGLFW();
		static bool InitializeGLEW();

		static void TerminateGLFW();

		static GLFWwindow* CreateWindow(int width, int height, const std::string& title, GLFWwindow* sharedWindow = NULL, GLFWmonitor* monitor = NULL, bool isHidden = false, bool isDecorated = true);
		static void SetSwapInterval(int interval);
		static void UseGLFWWindow(GLFWwindow* window);
		static void DestroyGLFWWindow(GLFWwindow* window);
	};
}