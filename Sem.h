#pragma once
#include "stdafx.h"
#include "Error.h"
#include <vector>
#include <map>

namespace Sem {
	void checkSemantic(LT::LexTable& lextable, IT::IdTable& idtable, std::map<std::string, std::vector<IT::IDDATATYPE>>& funcs);
}