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
	std::string::size_type fslashIndex = path.find_last_of('/');
	std::string::size_type bslashIndex = path.find_last_of('\\');

	if (fslashIndex == std::string::npos
		&& bslashIndex == std::string::npos)
		return path;

	if (fslashIndex < bslashIndex)
		fslashIndex = bslashIndex;

	fslashIndex += 1;
	return path.substr(fslashIndex, path.size() - fslashIndex);
}