#include "Sem.h"

namespace Sem {

// получить тип данных по индексу в таблице лексем
IT::IDDATATYPE getType(int idx, LT::LexTable& lextable, IT::IdTable& idtable) {
	if (lextable.table[idx].idxTI != LT_TI_NULLIDX) {
		return idtable.table[lextable.table[idx].idxTI].iddatatype;
	}
	return IT::IDDATATYPE::UNSIGNED;
}

// главная функция семантического анализа
void checkSemantic(LT::LexTable& lextable, IT::IdTable& idtable, std::map<std::string, std::vector<IT::IDDATATYPE>>& funcs) {
	bool flagMain = false;
	IT::IDDATATYPE currentFuncType = IT::IDDATATYPE::UNSIGNED; // тип возврата текущей функции
	bool insideFunction = false;

	for (int i = 0; i < lextable.size; i++) {
		// проверка наличия main и порядка объявления функций
		if (lextable.table[i].lexema[0] == LEX_MAIN) {
			flagMain = true;
			currentFuncType = IT::IDDATATYPE::UNSIGNED; // main всегда unsigned
			insideFunction = true;
		}

		if (lextable.table[i].lexema[0] == LEX_FUNC) {
			// функция не может быть объявлена после main
			if (flagMain) {
				throw ERROR_THROW_IN(102, lextable.table[i].sn, 0);
			}
			// запоминаем тип возврата текущей функции
			if (i + 2 < lextable.size) {
				currentFuncType = getType(i + 2, lextable, idtable);
				insideFunction = true;
			}
		}

		// проверка типа возвращаемого значения
		if (lextable.table[i].lexema[0] == LEX_SEND) {
			if (i + 1 < lextable.size) {
				IT::IDDATATYPE returnType = IT::IDDATATYPE::UNSIGNED;
				if (lextable.table[i + 1].lexema[0] == LEX_ID || lextable.table[i + 1].lexema[0] == LEX_LITERAL) {
					returnType = getType(i + 1, lextable, idtable);

					// разрешаем неявное приведение char к unsigned
					if (currentFuncType == IT::UNSIGNED && returnType == IT::CHAR) {}
					// в остальных случаях типы должны совпадать (упрощенная проверка)
					else if (currentFuncType != returnType) {
						if (lextable.table[i + 2].lexema[0] == LEX_SEMICOLON) {
							throw ERROR_THROW_IN(121, lextable.table[i].sn, 0);
						}
					}
				}
			}
		}

		// проверка параметров при вызове функции
		if (lextable.table[i].lexema[0] == LEX_ID && idtable.table[lextable.table[i].idxTI].idtype == IT::F) {

			// пропускаем объявление функции
			if (i > 0 && lextable.table[i - 1].lexema[0] == TYPE) continue;
			if (i > 1 && lextable.table[i - 2].lexema[0] == LEX_FUNC) continue;

			std::string funcName = idtable.table[lextable.table[i].idxTI].id;
			std::vector<IT::IDDATATYPE> expectedParams = funcs[funcName];

			int pIndex = 0;
			int k = i + 2;

			while (k < lextable.size && lextable.table[k].lexema[0] != LEX_RIGHTTHESIS) {
				if (lextable.table[k].lexema[0] == LEX_ID || lextable.table[k].lexema[0] == LEX_LITERAL) {

					// превышено количество параметров
					if (pIndex >= expectedParams.size()) {
						throw ERROR_THROW_IN(603, lextable.table[i].sn, 0);
					}

					// проверка типов параметров
					IT::IDDATATYPE actualType = getType(k, lextable, idtable);
					IT::IDDATATYPE expectedType = expectedParams[pIndex];

					if (actualType != expectedType) {
						// разрешаем неявное приведение char к unsigned
						if (!(expectedType == IT::UNSIGNED && actualType == IT::CHAR)) {
							throw ERROR_THROW_IN(122, lextable.table[k].sn, 0);
						}
					}
					pIndex++;
				}
				k++;
			}

			// недостаточно параметров
			if (pIndex < expectedParams.size()) {
				throw ERROR_THROW_IN(604, lextable.table[i].sn, 0);
			}
		}

		// проверка типов при присваивании и инициализации
		if (lextable.table[i].lexema[0] == LEX_EQUAL) {
			if (i > 0 && (lextable.table[i - 1].lexema[0] == LEX_ID)) {
				IT::IDDATATYPE leftType = getType(i - 1, lextable, idtable);

				// анализ выражения справа от знака равенства
				int j = i + 1;
				bool hasLogicOp = false;
				bool hasArithOp = false;

				while (j < lextable.size && lextable.table[j].lexema[0] != LEX_SEMICOLON) {
					char lex = lextable.table[j].lexema[0];

					// статическая проверка деления на ноль
					if (lex == LEX_COLON || lex == LEX_OST) {
						if (j + 1 < lextable.size && lextable.table[j + 1].lexema[0] == LEX_LITERAL) {
							if (idtable.table[lextable.table[j + 1].idxTI].value.vint == 0) {
								throw ERROR_THROW_IN(124, lextable.table[j].sn, 0);
							}
						}
					}

					// проверка совместимости типов
					if (lex == LEX_ID || lex == LEX_LITERAL) {
						IT::IDDATATYPE rightType = getType(j, lextable, idtable);
						if (leftType == IT::UNSIGNED && rightType == IT::LOGIC) {
							throw ERROR_THROW_IN(120, lextable.table[j].sn, 0);
						}
						if (leftType == IT::LOGIC && (rightType == IT::UNSIGNED || rightType == IT::CHAR)) {}
					}

					// смешивание арифметических и логических операций
					if (lex == LEX_MORE || lex == LEX_LESS || lex == LEX_ISEQUAL ||
						lex == LEX_NOT_EQUAL || lex == LEX_MORE_OR_EQUAL || lex == LEX_LESS_OR_EQUAL) {
						hasLogicOp = true;
					}
					if (lex == LEX_PLUS || lex == LEX_MINUS || lex == LEX_STAR || lex == LEX_COLON || lex == LEX_OST) {
						hasArithOp = true;
					}

					j++;
				}

				// проверка типа выражения
				if (leftType == IT::LOGIC) {
					if (hasArithOp && !hasLogicOp) {}
				}
			}
		}

		// проверка условий в if и because
		if (lextable.table[i].lexema[0] == LEX_IF || lextable.table[i].lexema[0] == LEX_BECAUSE) {
			int startCond = 0;
			int endCond = 0;

			// определение границ условного выражения
			// для if
			if (lextable.table[i].lexema[0] == LEX_IF) {
				startCond = i + 2;
				int brackets = 1;
				int k = startCond;
				while (k < lextable.size && brackets > 0) {
					k++;
					if (lextable.table[k].lexema[0] == LEX_LEFTTHESIS) brackets++;
					if (lextable.table[k].lexema[0] == LEX_RIGHTTHESIS) brackets--;
				}
				endCond = k;
			}
			// для because
			else if (lextable.table[i].lexema[0] == LEX_BECAUSE) {
				int k = i + 2;
				int semicolons = 0;
				while (k < lextable.size && semicolons < 1) {
					if (lextable.table[k].lexema[0] == LEX_SEMICOLON) semicolons++;
					k++;
				}
				startCond = k;
				while (k < lextable.size && lextable.table[k].lexema[0] != LEX_SEMICOLON) {
					k++;
				}
				endCond = k;
			}

			// анализ условного выражения
			bool hasComparison = false;
			bool isSingleLogic = false;

			if (endCond - startCond == 1) {
				IT::IDDATATYPE t = getType(startCond, lextable, idtable);
				if (t == IT::LOGIC) isSingleLogic = true;
			}

			for (int k = startCond; k < endCond; k++) {
				char lex = lextable.table[k].lexema[0];
				if (lex == LEX_MORE || lex == LEX_LESS || lex == LEX_ISEQUAL ||
					lex == LEX_NOT_EQUAL || lex == LEX_MORE_OR_EQUAL || lex == LEX_LESS_OR_EQUAL) {
					hasComparison = true;
				}
			}

			// условие не является логическим
			if (!hasComparison && !isSingleLogic) {
				throw ERROR_THROW_IN(606, lextable.table[i].sn, 0);
			}
		}
	}

	// проверка наличия main в коде
	if (!flagMain) {
		throw ERROR_THROW(103);
	}
}



}