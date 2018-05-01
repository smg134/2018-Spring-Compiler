#pragma once
#include "stdafx.h"
#include "Location.h"
#include "File.h"
#include <iostream>

std::ostream& operator<<(std::ostream& os, Location l) {
	if (l.source) {
		os << l.source->getPath();
	}
	else {
		os << "<input>";
	}
	os << ':' << l.line + 1 << ':' << l.column + 1;
	return os;
}