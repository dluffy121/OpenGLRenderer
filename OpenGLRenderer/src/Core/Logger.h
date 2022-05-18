#pragma once

#include <iostream>

#define ASSERT(x)	if(!(x)) __debugbreak();
#define GLLog(x)	GLClearError();\
					x;\
					ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#define DEBUGGING true
#define Log(x)		if(DEBUGGING) \
					std::cout << x << std::endl;

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);