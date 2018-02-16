#pragma once
#include <iosfwd>

class File;

class Location {
public:
	Location()
		: source(nullptr), line(-1), column(-1) {}

	Location(const File& f, int l, int c)
		: source(&f), line(l), column(c) {}
	
	explicit operator bool() const {
		return line >= 0;
	}

	const File* source;
	int line;
	int column;
};

std::ostream& operator<<(std::ostream& os, Location l);