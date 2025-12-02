#pragma once
#include "stdafx.h"
#include "Error.h"
#include <vector>

namespace SemA {
	void checkSemantic(LT::LexTable lextable, IT::IdTable idtable, map<string, vector<IT::IDDATATYPE>> funcs);
}