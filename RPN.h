#pragma once
#include "stdafx.h"
#include "LT.h"
#include "IT.h"
#include <stack>
#include <vector>

namespace RPN {
	// чем выше число, тем раньше выполняется операция.
	int getPriority(char lexema);

	bool Rpn(LT::LexTable& lextable, IT::IdTable& idtable, int startPos, int endPos);
	void searchAndConvert(LT::LexTable& lextable, IT::IdTable& idtable);
}