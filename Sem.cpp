#include "Sem.h"

// семантический анализ
namespace Sem {

// получить тип
IT::IDDATATYPE getType(int idx, LT::LexTable& lextable, IT::IdTable& idtable) {
	if (idx < 0 || idx >= lextable.size) return IT::IDDATATYPE::UNSIGNED;
	if (lextable.table[idx].idxTI != LT_TI_NULLIDX) {
		return idtable.table[lextable.table[idx].idxTI].iddatatype;
	}
	return IT::IDDATATYPE::UNSIGNED;
}

// проверка семантики
void checkSemantic(LT::LexTable& lextable, IT::IdTable& idtable, std::map<std::string, std::vector<IT::IDDATATYPE>>& funcs) {
	bool flagMain = false;
	IT::IDDATATYPE currentFuncType = IT::IDDATATYPE::UNSIGNED;
	bool insideFunction = false;
	std::map<int, int> knownValues; // отслеживание значений переменных

	for (int i = 0; i < lextable.size; i++) {
		// проверка наличия send в функциях
		if (lextable.table[i].lexema[0] == LEX_MAIN || lextable.table[i].lexema[0] == LEX_FUNC) {
			bool hasSend = false;
			int braceBalance = 0;
			int j = i;
			
			// ищем начало блока
			while (j < lextable.size && lextable.table[j].lexema[0] != LEX_LEFTBRACE) j++;
			if (j < lextable.size) {
				braceBalance = 1;
				j++;
				while (j < lextable.size && braceBalance > 0) {
					if (lextable.table[j].lexema[0] == LEX_LEFTBRACE) braceBalance++;
					if (lextable.table[j].lexema[0] == LEX_BRACELET) braceBalance--;
					if (lextable.table[j].lexema[0] == LEX_SEND) hasSend = true;
					j++;
				}
			}
			
			if (!hasSend) {
				throw ERROR_THROW_IN(600, lextable.table[i].sn, 0);
			}
		}

		// проверка main
		if (lextable.table[i].lexema[0] == LEX_MAIN) {
			flagMain = true;
			currentFuncType = IT::IDDATATYPE::UNSIGNED;
			insideFunction = true;
		}

		if (lextable.table[i].lexema[0] == LEX_FUNC) {
			// порядок функций
			if (flagMain) {
				throw ERROR_THROW_IN(302, lextable.table[i].sn, 0);
			}
			// тип возврата
			if (i + 2 < lextable.size) {
				currentFuncType = getType(i + 2, lextable, idtable);
				insideFunction = true;
			}
		}

		// возврат значения
		if (lextable.table[i].lexema[0] == LEX_SEND) {
			if (i + 1 < lextable.size) {
				IT::IDDATATYPE returnType = IT::IDDATATYPE::UNSIGNED;
				if (lextable.table[i + 1].lexema[0] == LEX_ID || lextable.table[i + 1].lexema[0] == LEX_LITERAL) {
					returnType = getType(i + 1, lextable, idtable);

					// char -> unsigned
					if (currentFuncType == IT::UNSIGNED && returnType == IT::CHAR) {}
					// проверка совпадения
					else if (currentFuncType != returnType) {
						if (lextable.table[i + 2].lexema[0] == LEX_SEMICOLON) {
							throw ERROR_THROW_IN(311, lextable.table[i].sn, 0);
						}
					}
				}
			}
		}

		// параметры вызова
		if (lextable.table[i].lexema[0] == LEX_ID && idtable.table[lextable.table[i].idxTI].idtype == IT::F) {

			if (i > 0 && lextable.table[i - 1].lexema[0] == TYPE) continue;
			if (i > 1 && lextable.table[i - 2].lexema[0] == LEX_FUNC) continue;

			std::string funcName = idtable.table[lextable.table[i].idxTI].id;
			std::vector<IT::IDDATATYPE> expectedParams = funcs[funcName];

			int pIndex = 0;
			int k = i + 2;

			while (k < lextable.size && lextable.table[k].lexema[0] != LEX_RIGHTTHESIS) {
				if (lextable.table[k].lexema[0] == LEX_ID || lextable.table[k].lexema[0] == LEX_LITERAL) {

					if (pIndex >= expectedParams.size()) {
						throw ERROR_THROW_IN(317, lextable.table[i].sn, 0);
					}

					IT::IDDATATYPE actualType = getType(k, lextable, idtable);
					IT::IDDATATYPE expectedType = expectedParams[pIndex];

					if (actualType != expectedType) {
						// char -> unsigned
						if (!(expectedType == IT::UNSIGNED && actualType == IT::CHAR)) {
							throw ERROR_THROW_IN(312, lextable.table[k].sn, 0);
						}
					}
					pIndex++;
				}
				k++;
			}

			if (pIndex < expectedParams.size()) {
				throw ERROR_THROW_IN(318, lextable.table[i].sn, 0);
			}
		}

		// присваивание
		if (lextable.table[i].lexema[0] == LEX_EQUAL) {
			if (i > 0 && (lextable.table[i - 1].lexema[0] == LEX_ID)) {
				IT::IDDATATYPE leftType = getType(i - 1, lextable, idtable);
				int lhsIdx = lextable.table[i - 1].idxTI;
				bool isSimple = false;
				int newVal = 0;

				// проверка на простое присваивание
				if (i + 2 < lextable.size && lextable.table[i + 2].lexema[0] == LEX_SEMICOLON) {
					if (lextable.table[i + 1].lexema[0] == LEX_LITERAL) {
						int litIdx = lextable.table[i + 1].idxTI;
						if (idtable.table[litIdx].iddatatype == IT::UNSIGNED) {
							isSimple = true;
							newVal = idtable.table[litIdx].value.vint;
						}
					} else if (lextable.table[i + 1].lexema[0] == LEX_ID) {
						int rhsIdx = lextable.table[i + 1].idxTI;
						if (knownValues.count(rhsIdx)) {
							isSimple = true;
							newVal = knownValues[rhsIdx];
						}
					}
				}

				// анализ правой части
				int j = i + 1;
				bool hasLogicOp = false;
				bool hasArithOp = false;

				while (j < lextable.size && lextable.table[j].lexema[0] != LEX_SEMICOLON) {
					char lex = lextable.table[j].lexema[0];

					// деление на ноль
					if (lex == LEX_COLON || lex == LEX_OST) {
						if (j + 1 < lextable.size) {
							if (lextable.table[j + 1].lexema[0] == LEX_LITERAL) {
								if (idtable.table[lextable.table[j + 1].idxTI].value.vint == 0) {
									throw ERROR_THROW_IN(314, lextable.table[j].sn, 0);
								}
							} else if (lextable.table[j + 1].lexema[0] == LEX_ID) {
								int opIdx = lextable.table[j + 1].idxTI;
								if (knownValues.count(opIdx) && knownValues[opIdx] == 0) {
									throw ERROR_THROW_IN(314, lextable.table[j].sn, 0);
								}
							}
						}
					}

					// совместимость
					if (lex == LEX_ID || lex == LEX_LITERAL) {
						IT::IDDATATYPE rightType = getType(j, lextable, idtable);
						if (leftType == IT::UNSIGNED && rightType == IT::LOGIC) {
							throw ERROR_THROW_IN(310, lextable.table[j].sn, 0);
						}
						if (leftType == IT::LOGIC && (rightType == IT::UNSIGNED || rightType == IT::CHAR)) {}
					}

					if (lex == LEX_MORE || lex == LEX_LESS || lex == LEX_ISEQUAL ||
						lex == LEX_NOT_EQUAL || lex == LEX_MORE_OR_EQUAL || lex == LEX_LESS_OR_EQUAL) {
						hasLogicOp = true;
					}
					if (lex == LEX_PLUS || lex == LEX_MINUS || lex == LEX_STAR || lex == LEX_COLON || lex == LEX_OST) {
						hasArithOp = true;
					}

					j++;
				}

				// обновление известных значений
				if (isSimple) knownValues[lhsIdx] = newVal;
				else knownValues.erase(lhsIdx);

				if (leftType == IT::LOGIC) {
					if (hasArithOp && !hasLogicOp) {}
				}
			}
		}

		// условия
		if (lextable.table[i].lexema[0] == LEX_IF || lextable.table[i].lexema[0] == LEX_BECAUSE) {
			int startCond = 0;
			int endCond = 0;

			// границы условия
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
			else if (lextable.table[i].lexema[0] == LEX_BECAUSE) {
				// Проверка: можно объявлять только unsigned integer в because
				int j = i + 2; // пропускаем because и (
				if (j + 2 < lextable.size && 
					(lextable.table[j].lexema[0] == LEX_UNSIGNED_INTEGER || // 't'
					 lextable.table[j].lexema[0] == LEX_CHAR ||             // 't'
					 lextable.table[j].lexema[0] == LEX_LOGIC) &&           // 't'
					lextable.table[j+1].lexema[0] == LEX_ID &&
					lextable.table[j+2].lexema[0] == LEX_EQUAL) {
					
					// Так как все типы имеют лексему 't', проверяем тип переменной в таблице идентификаторов
					IT::IDDATATYPE type = getType(j + 1, lextable, idtable);
					if (type != IT::UNSIGNED) {
						throw ERROR_THROW_IN(310, lextable.table[j].sn, 0);
					}
				}
				// Проверка: присваивание существующей переменной (должна быть unsigned integer)
				else if (j + 1 < lextable.size &&
					lextable.table[j].lexema[0] == LEX_ID &&
					lextable.table[j+1].lexema[0] == LEX_EQUAL) {
					
					IT::IDDATATYPE type = getType(j, lextable, idtable);
					if (type != IT::UNSIGNED) {
						throw ERROR_THROW_IN(310, lextable.table[j].sn, 0);
					}
				}

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

			// анализ
			bool hasComparison = false;
			bool isSingleLogic = false;
			bool hasId = false;

			if (endCond - startCond == 1) {
				IT::IDDATATYPE t = getType(startCond, lextable, idtable);
				if (t == IT::LOGIC) isSingleLogic = true;
			}

			for (int k = startCond; k < endCond; k++) {
				char lex = lextable.table[k].lexema[0];
				if (lex == LEX_ID) hasId = true;
				if (lex == LEX_MORE || lex == LEX_LESS || lex == LEX_ISEQUAL ||
					lex == LEX_NOT_EQUAL || lex == LEX_MORE_OR_EQUAL || lex == LEX_LESS_OR_EQUAL) {
					hasComparison = true;
				}
			}

			if (lextable.table[i].lexema[0] == LEX_BECAUSE && !hasId) {
				throw ERROR_THROW_IN(321, lextable.table[i].sn, 0);
			}

			if (!hasComparison && !isSingleLogic) {
				throw ERROR_THROW_IN(319, lextable.table[i].sn, 0);
			}
		}
	}

	// проверка main
	if (!flagMain) {
		throw ERROR_THROW(303);
	}
}



}