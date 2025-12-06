#pragma once
#include "stdafx.h"
#include "Error.h"
#include <vector>

namespace Sem {
	void checkSemantic(LT::LexTable lextable, IT::IdTable idtable, map<string, vector<IT::IDDATATYPE>> funcs);
}