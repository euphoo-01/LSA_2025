#pragma once
#include "stdafx.h"
#include <stack>
#include <queue>

namespace RPN {
	int getPriority(LT::Entry cur_lex);
	void Rpn(LT::LexTable lextable, IT::IdTable idtable, int curpos);
	void searchNextPosForCheck(LT::LexTable lextable, IT::IdTable idtable);
}