#include "stdafx.h"
#include "RPN.h"
#include <iostream>

using namespace std;

namespace RPN {
int getPriority(char lexema) {
	switch (lexema) {
	case LEX_LEFTTHESIS: return 0;
	case LEX_RIGHTTHESIS: return 0;
	case LEX_COMMA: return 1;

	case LEX_EQUAL: return 2;

	case LEX_MORE:
	case LEX_LESS:
	case LEX_ISEQUAL:
	case LEX_NOT_EQUAL:
	case LEX_MORE_OR_EQUAL:
	case LEX_LESS_OR_EQUAL: return 3;

	case LEX_PLUS:
	case LEX_MINUS: return 4;

	case LEX_STAR:
	case LEX_COLON:
	case LEX_OST:
		return 5;

	case LEX_BIT_NOT:
		return 6;

	default: return -1;
	}
}

bool Rpn(LT::LexTable& lextable, IT::IdTable& idtable, int startPos, int endPos) {
	stack<LT::Entry> stack;      // стек операторов
	vector<LT::Entry> outString; // выходная строка

	for (int i = startPos; i < endPos; i++) {
		LT::Entry entry = lextable.table[i];

		// игнорируем пустые лексемы
		if (entry.lexema[0] == '\0') continue;

		switch (entry.lexema[0]) {
		// операнды
		case LEX_LITERAL:
			outString.push_back(entry);
			break;

		case LEX_ID: {
			if (entry.idxTI != LT_TI_NULLIDX && idtable.table[entry.idxTI].idtype == IT::F) {
				stack.push(entry);
			}
			else {
				outString.push_back(entry);
			}
			break;
		}

		case LEX_LEFTTHESIS:
			stack.push(entry);
			break;

		case LEX_RIGHTTHESIS: {
			bool flag = false;
			while (!stack.empty()) {
				LT::Entry top = stack.top();
				stack.pop();
				if (top.lexema[0] == LEX_LEFTTHESIS) {
					flag = true;
					break;
				}
				outString.push_back(top);
			}
			if (!flag) {
				// ошибка баланса скобок
				return false;
			}
			// если после скобок шла функция выталкиваем её
			if (!stack.empty()) {
				if (stack.top().idxTI != LT_TI_NULLIDX &&
					idtable.table[stack.top().idxTI].idtype == IT::F) {
					outString.push_back(stack.top());
					stack.pop();
				}
			}
			break;
		}

		// разделитель аргументов функции
		case LEX_COMMA: {
			while (!stack.empty()) {
				if (stack.top().lexema[0] == LEX_LEFTTHESIS) {
					break;
				}
				outString.push_back(stack.top());
				stack.pop();
			}
			break;
		}

		// операторы
		case LEX_PLUS:
		case LEX_MINUS:
		case LEX_STAR:
		case LEX_COLON:
		case LEX_OST:
		case LEX_MORE:
		case LEX_LESS:
		case LEX_ISEQUAL:
		case LEX_NOT_EQUAL:
		case LEX_MORE_OR_EQUAL:
		case LEX_LESS_OR_EQUAL:
		case LEX_BIT_NOT:
		case LEX_EQUAL:
		{
			while (!stack.empty()) {
				int priorityStack = getPriority(stack.top().lexema[0]);
				int priorityCurrent = getPriority(entry.lexema[0]);

				// левоассоциативность
				if (stack.top().lexema[0] == LEX_LEFTTHESIS ||
				   (stack.top().idxTI != LT_TI_NULLIDX && idtable.table[stack.top().idxTI].idtype == IT::F)) {
					break;
				}

				if (priorityStack >= priorityCurrent) {
					outString.push_back(stack.top());
					stack.pop();
				}
				else {
					break;
				}
			}
			stack.push(entry);
			break;
		}

		default:
			break;
		}
	}

	// выталкиваем оставшееся из стека
	while (!stack.empty()) {
		outString.push_back(stack.top());
		stack.pop();
	}

	// записываем RPN
	for (int i = 0; i < outString.size(); i++) {
		lextable.table[startPos + i] = outString[i];
	}

	// оставшиеся ячейки заполняем пустышками
	for (int i = startPos + outString.size(); i < endPos; i++) {
		lextable.table[i].lexema[0] = '\0';
		lextable.table[i].idxTI = LT_TI_NULLIDX;
		lextable.table[i].sn = -1;
	}

	return true;
}

void searchAndConvert(LT::LexTable& lextable, IT::IdTable& idtable) {
	for (int i = 0; i < lextable.size; i++) {

		// если это объявление функции, пропускаем заголовок
		if (lextable.table[i].lexema[0] == LEX_FUNC) {
			while (lextable.table[i].lexema[0] != LEX_LEFTBRACE && i < lextable.size) {
				i++;
			}
			continue;
		}

		// x = выражение ;
		if (lextable.table[i].lexema[0] == LEX_EQUAL) {
			int start = i + 1; // Сразу после =
			int end = start;
			while (end < lextable.size && lextable.table[end].lexema[0] != LEX_SEMICOLON) {
				end++;
			}
			Rpn(lextable, idtable, start, end);
			i = end; // пропускаем обработанное
		}

		// send выражение ;
		else if (lextable.table[i].lexema[0] == LEX_SEND) {
			int start = i + 1;
			int end = start;
			while (end < lextable.size && lextable.table[end].lexema[0] != LEX_SEMICOLON) {
				end++;
			}
			Rpn(lextable, idtable, start, end);
			i = end;
		}

		// writech( выражение ) ;
		else if (lextable.table[i].lexema[0] == LEX_WRITECH) {
			if (i + 1 < lextable.size && lextable.table[i + 1].lexema[0] == LEX_LEFTTHESIS) {
				int start = i + 2;
				int end = start;
				int brackets = 1; // баланс скобок
				while (end < lextable.size) {
					if (lextable.table[end].lexema[0] == LEX_LEFTTHESIS) brackets++;
					if (lextable.table[end].lexema[0] == LEX_RIGHTTHESIS) brackets--;

					if (brackets == 0) break; // нашли закрывающую скобку
					end++;
				}
				Rpn(lextable, idtable, start, end);
				i = end;
			}
		}

		// if ( выражение )
		else if (lextable.table[i].lexema[0] == LEX_IF) {
			if (i + 1 < lextable.size && lextable.table[i + 1].lexema[0] == LEX_LEFTTHESIS) {
				int start = i + 2;
				int end = start;
				int brackets = 1;
				while (end < lextable.size) {
					if (lextable.table[end].lexema[0] == LEX_LEFTTHESIS) brackets++;
					if (lextable.table[end].lexema[0] == LEX_RIGHTTHESIS) brackets--;
					if (brackets == 0) break;
					end++;
				}
				Rpn(lextable, idtable, start, end);
				i = end;
			}
		}

		// because ( init ; condition ; step )
		else if (lextable.table[i].lexema[0] == LEX_BECAUSE) {
			// because ( ...
			int current = i + 2;
			int semicolonCount = 0;

			// пропускаем первую секцию до первой точки с запятой
			while (current < lextable.size) {
				if (lextable.table[current].lexema[0] == LEX_SEMICOLON) {
					semicolonCount++;
					if (semicolonCount == 1) break;
				}
				current++;
			}

			if (semicolonCount == 1) {
				int startCond = current + 1; // начало условия
				int endCond = startCond;

				// ищем конец условия
				while (endCond < lextable.size) {
					if (lextable.table[endCond].lexema[0] == LEX_SEMICOLON) break;
					endCond++;
				}

				Rpn(lextable, idtable, startCond, endCond);

				i = endCond;
			}
		}
	}
}
}