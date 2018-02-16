#pragma once
#include <string>

class File {
public:
	File(const std::string& path);

	const std::string& getPath() const;
	const std::string& getText() const;

private:
	std::string path;
	std::string text;
};