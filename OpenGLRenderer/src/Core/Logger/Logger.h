#pragma once

#include <assert.h>
#include <iostream>
#include <string>
#include <sstream>
#include <Macros.h>

namespace core
{
#define DEBUGGING true

#define ASSERT(...) EXPAND(GET_MACRO(__VA_ARGS__, ASSERT2, ASSERT1)(__VA_ARGS__))

#define ASSERT1(x)					\
					if(!(x)) __debugbreak();

#define ASSERT2(x, m)				\
					if(!__Assert(x, #x, __FILE__, __LINE__, m)) __debugbreak();

#define GLLog(x)					\
					GLClearError();	\
					x;				\
					assert(GLLogCall(#x, __FILE__, __LINE__));

#define Log(x)						\
					if(DEBUGGING)	\
					std::cout << x << std::endl;

#define LogError(x)					\
					if(DEBUGGING)	\
					std::cerr << x << std::endl;

	void GLClearError();
	bool GLLogCall(const char* function, const char* file, int line);
	bool __Assert(bool expr, const char* expr_str, const char* file, int line, const std::string& msg);
}