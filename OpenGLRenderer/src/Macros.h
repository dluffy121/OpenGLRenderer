#pragma once

#define ARRAYSIZE(a) \
  ((sizeof(a) / sizeof(*(a))) / \
  static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

#define __METHOD_NAME__ methodName(__PRETTY_FUNCTION__)

inline std::string methodName(const std::string& prettyFunction)
{
	size_t colons = prettyFunction.find("::");
	size_t begin = prettyFunction.substr(0, colons).rfind(" ") + 1;
	size_t end = prettyFunction.rfind("(") - begin;

	return prettyFunction.substr(begin, end) + "()";
}


#define __CLASS_NAME__ className(__PRETTY_FUNCTION__)

inline std::string className(const std::string& prettyFunction)
{
	size_t colons = prettyFunction.find("::");
	if (colons == std::string::npos)
		return "::";
	size_t begin = prettyFunction.substr(0, colons).rfind(" ") + 1;
	size_t end = colons - begin;

	return prettyFunction.substr(begin, end);
}