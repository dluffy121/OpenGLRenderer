#pragma once

#include <string>

static std::string GetParentDirectory(const std::string& path)
{
	std::string::size_type slashIndex = path.find_last_of('/');

	if (slashIndex == std::string::npos)
		return ".";

	if (slashIndex == 0)
		return "/";

	return path.substr(0, slashIndex);
}

static std::string GetFileName(const std::string& path)
{
	std::string::size_type slashIndex = path.find_last_of('/');

	if (slashIndex == std::string::npos)
		return path;

	slashIndex += 1;
	return path.substr(slashIndex, path.size() - slashIndex);
}