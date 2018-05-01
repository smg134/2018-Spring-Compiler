#pragma once
#include "stdafx.h"
#include "File.h"
#include <iterator>
#include <fstream>

File::File(const std::string& path) 
	: path(path) {
	std::ifstream inFile(path);
	std::istreambuf_iterator<char> first{ inFile };
	std::istreambuf_iterator<char> last{};
	text = std::string(first, last);
}

const std::string& File::getPath() const {
	return path;
}

const std::string& File::getText() const {
	return text;
}