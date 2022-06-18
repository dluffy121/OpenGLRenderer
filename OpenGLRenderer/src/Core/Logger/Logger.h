#pragma once

#include <assert.h>
#include <iostream>

namespace core
{
#define ASSERT(x)	if(!(x)) __debugbreak();
#define GLLog(x)	GLClearError();\
					x;\
					assert(GLLogCall(#x, __FILE__, __LINE__))

#define DEBUGGING true
#define Log(x)		if(DEBUGGING) \
					std::cout << x << std::endl;

	void GLClearError();
	bool GLLogCall(const char* function, const char* file, int line);
}
