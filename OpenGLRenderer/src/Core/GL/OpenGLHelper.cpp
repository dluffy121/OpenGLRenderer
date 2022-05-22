#include "OpenGLHelper.h"
#include "../Logger.h"
#include <stdio.h>

unsigned int GetSizeOfType(unsigned int type)
{
	switch (type)
	{
	case GL_FLOAT:			return 4;
	case GL_UNSIGNED_INT:	return 4;
	case GL_UNSIGNED_SHORT:	return 2;
	case GL_UNSIGNED_BYTE:	return 1;
	}
	ASSERT(false);
	return 0;
}

void OpenGLHelper::TerminateGLFW()
{
	std::cout << "GLFW Terminated" << std::endl;
	glfwTerminate();
}

int OpenGLHelper::InitializeGLFW()
{
	int retVal = glfwInit();
	fprintf(stdout, "Status: Using GLFW %s\n", glfwGetVersionString());
	return retVal;
}

bool OpenGLHelper::InitializeGLEW()
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		return false;
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	return true;
}

GLFWwindow* OpenGLHelper::CreateWindow(int width, int height, const std::string& title, GLFWwindow* sharedWindow, bool isHidden, bool isDecorated)
{
	glfwWindowHint(GLFW_VISIBLE, isHidden ? GLFW_FALSE : GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, isDecorated ? GLFW_TRUE : GLFW_FALSE);
	return glfwCreateWindow(width, height, title.c_str(), NULL, sharedWindow);
}

void OpenGLHelper::SetSwapInterval(int interval)
{
	glfwSwapInterval(interval);			// sets swap interval current gl context window i.e. wait for screen updates https://www.glfw.org/docs/3.3/group__context.html#ga6d4e0cdf151b5e579bd67f13202994ed
}

void OpenGLHelper::UseGLFWWindow(GLFWwindow* window)
{
	glfwMakeContextCurrent(window);
}

void OpenGLHelper::DestroyGLFWWindow(GLFWwindow* window)
{
	glfwDestroyWindow(window);
}