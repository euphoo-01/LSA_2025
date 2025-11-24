#include "../Headers/SemA.h"
#include <vector>

namespace SemA {
	bool flagMain = false;
	int curpos = 0;
	int curline = 1;
	void checkSemantic(LT::LexTable lextable, IT::IdTable idtable, map<string, vector<IT::IDDATATYPE>> funcs) {
		for (int i = 0; i < lextable.size; i++) {
			if (curline != lextable.table[i].sn) {
				curline = lextable.table[i].sn;
				curpos = 0;
			}
			curpos++;
			if (lextable.table[i].lexema[0] == LEX_MAIN) {
				flagMain = true;
			}
			// проверка того, что все функции должны быть объявлены до main
			if (lextable.table[i].lexema[0] == LEX_FUNCTION && flagMain) { 
				throw ERROR_THROW_IN(102, curline, curpos);
			}
			//if (lextable.table[i].lexema[0] == '=') {
			//	if (idtable.table[lextable.table[i - 1].idxTI].iddatatype != idtable.table[lextable.table[i + 1].idxTI].iddatatype) {
			//		throw ERROR_THROW_IN(120, curline, curpos);
			//	}
			//}
			// проверка на соответствие типа функции возвращаемому типу
			if ((lextable.table[i].lexema[0] == LEX_ID && idtable.table[lextable.table[i].idxTI].idtype == IT::F && lextable.table[i - 1].lexema[0] == TYPE) || lextable.table[i].lexema[0] == LEX_MAIN) { 
				int j = i;
				int localcurpos = 0;
				while (lextable.table[j].lexema[0] != LEX_RETURN) {
					if (lextable.table[j - 1].sn != lextable.table[j].sn) {
						localcurpos = 0;
					}
					j++;
					localcurpos++;
				}
				if (idtable.table[lextable.table[j + 1].idxTI].iddatatype != idtable.table[lextable.table[i].idxTI].iddatatype) {
					throw ERROR_THROW_IN(121, lextable.table[j + 1].sn, localcurpos);
				}
			}
			// проверка на соответствие типов фактических и формальных параметров, а также на количество параметров
			if ((lextable.table[i].lexema[0] == LEX_ID && idtable.table[lextable.table[i].idxTI].idtype == IT::F && lextable.table[i - 1].lexema[0] != TYPE) && lextable.table[i].lexema[0] != LEX_MAIN) {
				int indexparm = 0;
				for (int k = 1; lextable.table[i + k].lexema[0] != LEX_RIGHTTHESIS; k++) {
					if (lextable.table[i + k].lexema[0] == LEX_ID || lextable.table[i + k].lexema[0] == LEX_LITERAL)
					{
						if (indexparm >= funcs[idtable.table[lextable.table[i].idxTI].id].size()) {
							throw ERROR_THROW_IN(122, curline, curpos + k);
						}
						if (idtable.table[lextable.table[i + k].idxTI].iddatatype != funcs[idtable.table[lextable.table[i].idxTI].id][indexparm]) {
							throw ERROR_THROW_IN(122, curline, curpos + k);
						}
						indexparm++;
					}
				}
			}
		}
		// проверка на наличие main
		if (!flagMain) {
			throw ERROR_THROW(103);
		}

		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexema[0] == LEX_EQUAL)
			{
				// проверка на то, что мы присваиваем значение переменной или параметру функции
				if (idtable.table[lextable.table[i - 1].idxTI].idtype != IT::IDTYPE::V && idtable.table[lextable.table[i - 1].idxTI].idtype != IT::IDTYPE::P)
					throw ERROR_THROW_IN(123, lextable.table[i - 1].sn, 0);
				IT::IDDATATYPE curtype = idtable.table[lextable.table[i - 1].idxTI].iddatatype;

				int indexB = 0;
				bool isFirst = true;
				IT::IDDATATYPE type1 = IT::IDDATATYPE::UINT;

				switch (curtype)
				{
				case IT::IDDATATYPE::BOOL: {

					while (lextable.table[i + indexB].lexema[0] != LEX_SEMICOLON)
					{
						if (lextable.table[i + indexB].lexema[0] == LEX_PLUS || lextable.table[i + indexB].lexema[0] == LEX_MINUS
							|| lextable.table[i + indexB].lexema[0] == LEX_DIRSLASH || lextable.table[i + indexB].lexema[0] == LEX_STAR
							|| lextable.table[i + indexB].lexema[0] == LEX_OST) {
							throw ERROR_THROW_IN(602, lextable.table[i + indexB].sn, 0);
						}

						if (lextable.table[i + indexB].lexema[0] == LEX_ID || lextable.table[i + indexB].lexema[0] == LEX_LITERAL)
						{
							if (isFirst)
							{
								type1 = idtable.table[lextable.table[i + indexB].idxTI].iddatatype;
								isFirst = false;
								if (lextable.table[i + indexB + 1].lexema[0] == LEX_SEMICOLON && type1 != IT::BOOL) {
									throw ERROR_THROW_IN(120, lextable.table[i + indexB].sn, 0);
								}
							}

							if (idtable.table[lextable.table[i + indexB].idxTI].idtype == IT::IDTYPE::F)
							{
								if (idtable.table[lextable.table[i + indexB].idxTI].iddatatype != curtype)
									throw ERROR_THROW_IN(120, lextable.table[i + indexB].sn, 0);
								int countOfHesis = 0;
								if (lextable.table[i + indexB + 1].lexema[0] != LEX_LEFTTHESIS)
									throw ERROR_THROW_IN(602, lextable.table[i + indexB].sn, 0);
								do
								{
									indexB++;
									if (lextable.table[i + indexB].lexema[0] == LEX_LEFTTHESIS)
										countOfHesis++;
									if (lextable.table[i + indexB].lexema[0] == LEX_RIGHTTHESIS)
										countOfHesis--;
								} while (countOfHesis != 0);
							}
							else if (idtable.table[lextable.table[i + indexB].idxTI].iddatatype != type1)
								throw ERROR_THROW_IN(120, lextable.table[i + indexB].sn, 0);
						}
						indexB++;
						continue;
					}
					break;
				}
				case IT::IDDATATYPE::STR: {
					for (int j = 0; lextable.table[i + j].lexema[0] != LEX_SEMICOLON; j++)
					{
						if (lextable.table[i + j].lexema[0] == LEX_PLUS || lextable.table[i + j].lexema[0] == LEX_MINUS
							|| lextable.table[i + j].lexema[0] == LEX_DIRSLASH || lextable.table[i + j].lexema[0] == LEX_STAR
							|| lextable.table[i + j].lexema[0] == LEX_LESS
							|| lextable.table[i + j].lexema[0] == LEX_MORE || lextable.table[i + j].lexema[0] == LEX_ISEQUAL
							|| lextable.table[i + j].lexema[0] == LEX_NOT_EQUAL || lextable.table[i + j].lexema[0] == LEX_LESS_OR_EQUAL
							|| lextable.table[i + j].lexema[0] == LEX_MORE_OR_EQUAL || lextable.table[i + j].lexema[0] == LEX_OST 
							|| lextable.table[i + j].lexema[0] == LEX_OR || lextable.table[i + j].lexema[0] == LEX_AND)
							throw ERROR_THROW_IN(602, lextable.table[i + j].sn, 0);
						if (lextable.table[i + j].lexema[0] == LEX_ID || lextable.table[i + j].lexema[0] == LEX_LITERAL)
						{
							if (idtable.table[lextable.table[i + j].idxTI].idtype == IT::IDTYPE::F)
							{
								if (idtable.table[lextable.table[i + j].idxTI].iddatatype != curtype)
									throw ERROR_THROW_IN(120, lextable.table[i + j].sn, 0);
								int countOfHesis = 0;
								if (lextable.table[i + j + 1].lexema[0] != LEX_LEFTTHESIS)
									throw ERROR_THROW_IN(602, lextable.table[i + j].sn, 0);
								do
								{
									j++;
									if (lextable.table[i + j].lexema[0] == LEX_LEFTTHESIS)
										countOfHesis++;
									if (lextable.table[i + j].lexema[0] == LEX_RIGHTTHESIS)
										countOfHesis--;
								} while (countOfHesis != 0);
							}
							else if (idtable.table[lextable.table[i + j].idxTI].iddatatype != curtype)
								throw ERROR_THROW_IN(120, lextable.table[i + j].sn, 0);
						}
					}
					break;
				}
				case IT::IDDATATYPE::UINT: {
					int index = 0;
					while (lextable.table[i + index].lexema[0] != LEX_SEMICOLON)
					{
						if ((lextable.table[i + index].lexema[0] == LEX_DIRSLASH || lextable.table[i + index].lexema[0] == LEX_OST) && idtable.table[lextable.table[i + index + 1].idxTI].value.vint == 0)
						{
							throw ERROR_THROW_IN(124, lextable.table[i + index].sn, 0);
						}
						if (lextable.table[i + index].lexema[0] == LEX_LESS || lextable.table[i + index].lexema[0] == LEX_MORE
							|| lextable.table[i + index].lexema[0] == LEX_ISEQUAL || lextable.table[i + index].lexema[0] == LEX_NOT_EQUAL)
							throw ERROR_THROW_IN(602, lextable.table[i + index].sn, 0);
						if (lextable.table[i + index].lexema[0] == LEX_ID || lextable.table[i + index].lexema[0] == LEX_LITERAL)
						{
							if (idtable.table[lextable.table[i + index].idxTI].idtype == IT::IDTYPE::F)
							{
								if (idtable.table[lextable.table[i + index].idxTI].iddatatype != curtype)
									throw ERROR_THROW_IN(120, lextable.table[i + index].sn, 0);
								int countOfHesis = 0;
								if (lextable.table[i + index + 1].lexema[0] != LEX_LEFTTHESIS)
									throw ERROR_THROW_IN(602, lextable.table[i + index].sn, 0);
								do
								{
									index++;
									if (lextable.table[i + index].lexema[0] == LEX_LEFTTHESIS)
										countOfHesis++;
									if (lextable.table[i + index].lexema[0] == LEX_RIGHTTHESIS)
										countOfHesis--;
								} while (countOfHesis != 0);
							}
							else
							{
								if (idtable.table[lextable.table[i + index].idxTI].iddatatype != curtype)
									throw ERROR_THROW_IN(120, lextable.table[i + index].sn, 0);
							}
						}
						index++;
						continue;
					}
					break;
				}
				case IT::IDDATATYPE::DOUBLE: {
					int indexD = 0;
					while (lextable.table[i + indexD].lexema[0] != LEX_SEMICOLON)
					{
						if ((lextable.table[i + indexD].lexema[0] == LEX_DIRSLASH) && idtable.table[lextable.table[i + indexD + 1].idxTI].value.vint == 0.0)
						{
							throw ERROR_THROW_IN(124, lextable.table[i + indexD].sn, 0);
						}
						if (lextable.table[i + indexD].lexema[0] == LEX_LESS || lextable.table[i + indexD].lexema[0] == LEX_MORE
							|| lextable.table[i + indexD].lexema[0] == LEX_ISEQUAL || lextable.table[i + indexD].lexema[0] == LEX_NOT_EQUAL || lextable.table[i + indexD].lexema[0] == LEX_OST
							|| lextable.table[i + indexD].lexema[0] == LEX_OR || lextable.table[i + indexD].lexema[0] == LEX_AND)
							throw ERROR_THROW_IN(602, lextable.table[i + indexD].sn, 0);
						if (lextable.table[i + indexD].lexema[0] == LEX_ID || lextable.table[i + indexD].lexema[0] == LEX_LITERAL)
						{
							if (idtable.table[lextable.table[i + indexD].idxTI].idtype == IT::IDTYPE::F)
							{
								if (idtable.table[lextable.table[i + indexD].idxTI].iddatatype != curtype)
									throw ERROR_THROW_IN(120, lextable.table[i + indexD].sn, 0);
								int countOfHesis = 0;
								if (lextable.table[i + indexD + 1].lexema[0] != LEX_LEFTTHESIS)
									throw ERROR_THROW_IN(602, lextable.table[i + indexD].sn, 0);
								do
								{
									indexD++;
									if (lextable.table[i + indexD].lexema[0] == LEX_LEFTTHESIS)
										countOfHesis++;
									if (lextable.table[i + indexD].lexema[0] == LEX_RIGHTTHESIS)
										countOfHesis--;
								} while (countOfHesis != 0);
							}
							else
							{
								if (idtable.table[lextable.table[i + indexD].idxTI].iddatatype != curtype)
									throw ERROR_THROW_IN(120, lextable.table[i + indexD].sn, 0);
							}
						}
						indexD++;
						continue;
					}
					break;
				}
				}
			}
			if (lextable.table[i].lexema[0] == LEX_IF)
			{
				if (idtable.table[lextable.table[i + 2].idxTI].iddatatype == IT::IDDATATYPE::STR || idtable.table[lextable.table[i + 4].idxTI].iddatatype == IT::IDDATATYPE::STR)
				{
					throw ERROR_THROW_IN(606, lextable.table[i].sn, 0);
				}
				else if (idtable.table[lextable.table[i + 2].idxTI].iddatatype == IT::IDDATATYPE::UINT && idtable.table[lextable.table[i + 4].idxTI].iddatatype == IT::IDDATATYPE::UINT)
				{
					if (!(lextable.table[i + 3].lexema[0] == LEX_MORE || lextable.table[i + 3].lexema[0] == LEX_LESS || lextable.table[i + 3].lexema[0] == LEX_MORE_OR_EQUAL || lextable.table[i + 3].lexema[0] == LEX_LESS_OR_EQUAL ||
						lextable.table[i + 3].lexema[0] == LEX_ISEQUAL || lextable.table[i + 3].lexema[0] == LEX_NOT_EQUAL))
						throw ERROR_THROW_IN(606, lextable.table[i].sn, 0);
				}
				else if (idtable.table[lextable.table[i + 2].idxTI].iddatatype == IT::IDDATATYPE::DOUBLE && idtable.table[lextable.table[i + 4].idxTI].iddatatype == IT::IDDATATYPE::DOUBLE)
				{
					if (!(lextable.table[i + 3].lexema[0] == LEX_MORE || lextable.table[i + 3].lexema[0] == LEX_LESS || lextable.table[i + 3].lexema[0] == LEX_MORE_OR_EQUAL || lextable.table[i + 3].lexema[0] == LEX_LESS_OR_EQUAL ||
						lextable.table[i + 3].lexema[0] == LEX_ISEQUAL || lextable.table[i + 3].lexema[0] == LEX_NOT_EQUAL))
						throw ERROR_THROW_IN(606, lextable.table[i].sn, 0);
				}
				else if (idtable.table[lextable.table[i + 2].idxTI].iddatatype == IT::IDDATATYPE::BOOL && idtable.table[lextable.table[i + 4].idxTI].iddatatype == IT::IDDATATYPE::BOOL)
				{
					if (!(lextable.table[i + 3].lexema[0] == LEX_MORE || lextable.table[i + 3].lexema[0] == LEX_LESS || lextable.table[i + 3].lexema[0] == LEX_MORE_OR_EQUAL || lextable.table[i + 3].lexema[0] == LEX_LESS_OR_EQUAL ||
						lextable.table[i + 3].lexema[0] == LEX_ISEQUAL || lextable.table[i + 3].lexema[0] == LEX_NOT_EQUAL))
						throw ERROR_THROW_IN(606, lextable.table[i].sn, 0);
				}
				else {
					throw ERROR_THROW_IN(606, lextable.table[i].sn, 0);
				}
			}
			if (lextable.table[i].lexema[0] == LEX_WHILE)
			{
				if (idtable.table[lextable.table[i + 2].idxTI].iddatatype == IT::IDDATATYPE::STR || idtable.table[lextable.table[i + 4].idxTI].iddatatype == IT::IDDATATYPE::STR)
				{
					throw ERROR_THROW_IN(606, lextable.table[i].sn, 0);
				}
				else if (idtable.table[lextable.table[i + 2].idxTI].iddatatype == IT::IDDATATYPE::UINT && idtable.table[lextable.table[i + 4].idxTI].iddatatype == IT::IDDATATYPE::UINT)
				{
					if (!(lextable.table[i + 3].lexema[0] == LEX_MORE || lextable.table[i + 3].lexema[0] == LEX_LESS || lextable.table[i + 3].lexema[0] == LEX_MORE_OR_EQUAL || lextable.table[i + 3].lexema[0] == LEX_LESS_OR_EQUAL ||
						lextable.table[i + 3].lexema[0] == LEX_ISEQUAL || lextable.table[i + 3].lexema[0] == LEX_NOT_EQUAL))
						throw ERROR_THROW_IN(606, lextable.table[i].sn, 0);
				}
				else if (idtable.table[lextable.table[i + 2].idxTI].iddatatype == IT::IDDATATYPE::DOUBLE && idtable.table[lextable.table[i + 4].idxTI].iddatatype == IT::IDDATATYPE::DOUBLE)
				{
					if (!(lextable.table[i + 3].lexema[0] == LEX_MORE || lextable.table[i + 3].lexema[0] == LEX_LESS || lextable.table[i + 3].lexema[0] == LEX_MORE_OR_EQUAL || lextable.table[i + 3].lexema[0] == LEX_LESS_OR_EQUAL ||
						lextable.table[i + 3].lexema[0] == LEX_ISEQUAL || lextable.table[i + 3].lexema[0] == LEX_NOT_EQUAL))
						throw ERROR_THROW_IN(606, lextable.table[i].sn, 0);
				}
				else if(idtable.table[lextable.table[i + 2].idxTI].iddatatype == IT::IDDATATYPE::BOOL && idtable.table[lextable.table[i + 4].idxTI].iddatatype == IT::IDDATATYPE::BOOL)
				{
					if (!(lextable.table[i + 3].lexema[0] == LEX_MORE || lextable.table[i + 3].lexema[0] == LEX_LESS || lextable.table[i + 3].lexema[0] == LEX_MORE_OR_EQUAL || lextable.table[i + 3].lexema[0] == LEX_LESS_OR_EQUAL ||
						lextable.table[i + 3].lexema[0] == LEX_ISEQUAL || lextable.table[i + 3].lexema[0] == LEX_NOT_EQUAL || lextable.table[i + 3].lexema[0] == LEX_OR || lextable.table[i + 3].lexema[0] == LEX_AND))
						throw ERROR_THROW_IN(606, lextable.table[i].sn, 0);
				}
				else {
					throw ERROR_THROW_IN(606, lextable.table[i].sn, 0);
				}
			}
		}
	}
}