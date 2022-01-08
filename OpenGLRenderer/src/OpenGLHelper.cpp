#pragma once

#include <GL/glew.h>
#include "OpenGLHelper.h"
#include <stdio.h>

OpenGLHelper::OpenGLHelper()
{}

OpenGLHelper* OpenGLHelper::getOGHInstance()
{
	static OpenGLHelper instance;
	return &instance;
}

int OpenGLHelper::Init(GLFWwindow*& window)
{
	if (!InitializeGLFW())
		return -1;

	window = CreateWindow();
	if (!window)
	{
		TerminateGLFW();
		return -1;
	}

	MakeWindow(window);

	if (!InitializeGLEW())
		return -1;

	fprintf(stdout, "Status: Using GL %s\n", glGetString(GL_VERSION));

	return 0;
}

void OpenGLHelper::TerminateGLFW()
{
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

GLFWwindow* OpenGLHelper::CreateWindow()
{
	return glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
}

void OpenGLHelper::MakeWindow(GLFWwindow* window)
{
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);			// sets swap interval current gl context window i.e. wait for screen updates https://www.glfw.org/docs/3.3/group__context.html#ga6d4e0cdf151b5e579bd67f13202994ed
}