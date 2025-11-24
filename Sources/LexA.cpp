#include"../Headers/LexA.h"
#include "../stdafx.h"
#include"../Headers/In.h"
#include"../Headers/Error.h"
#include <stdio.h>
#include <string.h>
#include <iomanip>
#include <vector>
#include <map>

namespace LexA
{
	LT::LexTable lexTable = LT::Create(LT_MAXSIZE - 1);
	IT::IdTable idTable = IT::Create(TI_MAXSIZE - 1);
	int countParms = 0;
	bool whileflag = false;
	bool ifflag = false;
	bool elseflag = false;
	bool stringFlag = false;
	bool uintFlag = false;
	bool boolFlag = false;
	bool doubleFlag = false;
	bool parmFlag = false;
	bool litflag = false;
	bool falseFlag = false;
	bool trueFlag = false;
	bool mainFlag = false;
	bool callFunc;
	char* str = new char[TI_MAXSIZE];
	bool declareFlag = false;
	bool declareFunc = false;
	bool pushToIT = false;

	void Add_LT(int line, char token)
	{
		LT::Entry entry;
		entry.sn = line;
		memset(entry.lexema, '\0', strlen(entry.lexema) - 1);
		entry.lexema[0] = token;
		LT::Add(lexTable, entry);
	}
	char FST()
	{
		FST_UINT
		FST_STRING
		FST_BOOL
		FST_FUNC
		FST_RETURN
		FST_MAIN
		FST_PRINT
		FST_PRINTLN
		FST_READ
		FST_TRUE
		FST_FALSE
		FST_WHILE
		FST_IF
		FST_LITERAL
		FST_IDEN
		FST_DOUBLE
		FST_ELSE
		FST_UNTIL
		FST_OTHERWISE
		if (FST::execute(_uint)) {
			uintFlag = true;
			return LEX_UINTEGER;
		}
		if (FST::execute(_double)) {
			doubleFlag = true;
			return LEX_DOUBLE;
		}
		if (FST::execute(_bool)) {
			boolFlag = true;
			return LEX_BOOL;
		}
		if (FST::execute(_string))
		{
			stringFlag = true;
			return LEX_STRING;
		}
		if (FST::execute(_function)) {
			return LEX_FUNCTION;
		}
		if (FST::execute(_return))
			return LEX_RETURN;
		if (FST::execute(_main))
		{
			mainFlag = true;
			return LEX_MAIN;
		}
		if (FST::execute(_print))
			return LEX_PRINT;
		if (FST::execute(_println))
			return LEX_PRINTLN;
		if (FST::execute(_read))
			return LEX_READ;
		if (FST::execute(_true)) {
			trueFlag = true;
			return LEX_LITERAL;
		}
		if (FST::execute(_false)) {
			falseFlag = true;
			return LEX_LITERAL;
		}
		if (FST::execute(_while)) {
			whileflag = true;
			return LEX_WHILE;
		}
		if (FST::execute(_if)) {
			ifflag = true;
			return LEX_IF;
		}
		if (FST::execute(_else)) {
			elseflag = true;
			return LEX_ELSE;
		}
		if (FST::execute(_until)) {
			whileflag = true;
			return LEX_UNTIL;
		}
		if (FST::execute(_otherwise)) {
			elseflag = true;
			return LEX_OTHERWISE;
		}
		if (FST::execute(_literal))
			return LEX_LITERAL;
		if (FST::execute(_iden))
			return LEX_ID;
		return '\0';
	}
	LEX LexA(Parm::PARm parm, In::iN in)
	{
		LEX lexResult;
		IT::Add(idTable, *new IT::Entry(-1, "compare", IT::UINT, IT::F, "global", 2));
		IT::Add(idTable, *new IT::Entry(-1, "len", IT::UINT, IT::F, "global", 1));
		IT::Add(idTable, *new IT::Entry(-1, "getmin", IT::UINT, IT::F, "global", 2));
		IT::Add(idTable, *new IT::Entry(-1, "getmax", IT::UINT, IT::F, "global", 2));
		lexResult.functions["compare"].push_back(IT::STR);
		lexResult.functions["compare"].push_back(IT::STR);
		lexResult.functions["getmin"].push_back(IT::UINT);
		lexResult.functions["getmin"].push_back(IT::UINT);
		lexResult.functions["getmax"].push_back(IT::UINT);
		lexResult.functions["getmax"].push_back(IT::UINT);
		lexResult.functions["len"].push_back(IT::STR);
		int indexIT;
		int pos = 0;
		LT::Entry cur_lex;
		int bufferIndex = 0;
		cur_lex.sn = 0;
		cur_lex.idxTI = 0;
		cur_lex.lexema[0] = '\0';
		int number_literal = 0;
		int number_until = 0;
		int number_if = 0;
		int number_else = 0;
		map<string, string> scope;
		scope["global"] = "";
		string cur_scope = "global";
		string prev_scope = "";
		IT::Entry cur_iden;
		lexTable.size = 0;
		int currentLine = 1;
		ofstream LT_file;
		ofstream IT_file;
		LT_file.open("LT.txt");
		IT_file.open("IT.txt");
		for (int i = 0; i < in.size; i++)
		{
			pos++;
 			if (((in.text[i] >= 'A' && in.text[i] <= 'Z') || (in.text[i] >= 'a' && in.text[i] <= 'z') || (in.text[i] >= '0' && in.text[i] <= '9') || (in.text[i] >= 0xC0 && in.text[i] <= 0xFF) || (in.text[i] >= 0xE0 && in.text[i] <= 0xFF) || in.text[i] == DOT) && in.text[i] != SPACE || (in.text[i] == MARK && !litflag) || (in.text[i] != MARK && litflag))
			{
				if (in.text[i] == MARK) {
					litflag = true;
				}
				str[bufferIndex] = in.text[i];
				bufferIndex++;
				if (bufferIndex >= TI_MAXSIZE)
				{
					throw ERROR_THROW(119);
				}
			}
			else
			{
				if (bufferIndex > 0) {
					str[bufferIndex] = '\0';
					cur_lex.lexema[0] = FST();
				} else {
					cur_lex.lexema[0] = '\0';
				}
				
				// Обработка одиночных символов
				if (bufferIndex == 0) {
					switch (in.text[i]) {
						case LEX_LEFTBRACE:
							cur_lex.lexema[0] = LEX_LEFTBRACE;
							break;
						case LEX_BRACELET:
							cur_lex.lexema[0] = LEX_BRACELET;
							break;
						case LEX_SEMICOLON:
							cur_lex.lexema[0] = LEX_SEMICOLON;
							break;
						case LEX_COMMA:
							cur_lex.lexema[0] = LEX_COMMA;
							break;
						case LEX_LEFTTHESIS:
							cur_lex.lexema[0] = LEX_LEFTTHESIS;
							break;
						case LEX_RIGHTTHESIS:
							cur_lex.lexema[0] = LEX_RIGHTTHESIS;
							break;
						case LEX_PLUS:
							cur_lex.lexema[0] = LEX_PLUS;
							break;
						case LEX_MINUS:
							cur_lex.lexema[0] = LEX_MINUS;
							break;
						case LEX_STAR:
							cur_lex.lexema[0] = LEX_STAR;
							break;
						case LEX_DIRSLASH:
							cur_lex.lexema[0] = LEX_DIRSLASH;
							break;
						case LEX_EQUAL:
							cur_lex.lexema[0] = LEX_EQUAL;
							break;
						case LEX_OST:
							cur_lex.lexema[0] = LEX_OST;
							break;
						default:
							cur_lex.lexema[0] = '\0';
							break;
					}
				}
				// cout << "Текущая область видимости " << cur_scope << " Предыдущая " << prev_scope << '\n';
				if (cur_lex.lexema[0] == LEX_MAIN)
				{
					if (callFunc) {
						throw ERROR_THROW_IN(600, currentLine, 0);
					}
					mainFlag = true;
					cur_lex.idxTI = idTable.size;
					memcpy(cur_iden.id, str, 15);
					cur_iden.id[15] = '\0';
					cur_iden.iddatatype = IT::UINT;
					cur_iden.idtype = IT::F;
                                        cur_iden.value.vint = 0.0;
					cur_iden.idxfirstLE = currentLine;
					cur_iden.scope_name = cur_scope;
					indexIT = IT::search(idTable, cur_iden);
					if (indexIT >= 0)
					{
						throw ERROR_THROW_IN(109, currentLine, pos);
					}
					if (indexIT == -1)
					{
						IT::Add(idTable, cur_iden);
					}
				}
				if (cur_lex.lexema[0] == LEX_LITERAL)
				{
					cur_iden.iddatatype = IT::UINT;
					cur_iden.idtype = IT::L;
					std::sprintf(cur_iden.id, "");
					bool dot = false;
					for (int j = 0; j < strlen(str); j++) {
						if (str[j] == '.') {
							dot = true;
							break;
						}
					}
					if (trueFlag) {
						cur_iden.iddatatype = IT::BOOL;
						cur_iden.value.vint = 1;
					}
					else if (falseFlag) {
						cur_iden.iddatatype = IT::BOOL;
						cur_iden.value.vint = 0;
					}
					else if (dot) {
						cur_iden.iddatatype = IT::DOUBLE;
						if ((lexTable.table[lexTable.size - 2].lexema[0] != LEX_RIGHTTHESIS 
							&& lexTable.table[lexTable.size - 2].lexema[0] != LEX_ID && lexTable.table[lexTable.size - 2].lexema[0] != LEX_LITERAL)
							&& in.text[i - strlen(str)] != ' ' && in.text[i - strlen(str) - 1] == LEX_MINUS) {
							std::sscanf(str, "%lf", &cur_iden.value.vint);
							cur_iden.value.vint = -cur_iden.value.vint;
						}
						else {
							std::sscanf(str, "%lf", &cur_iden.value.vint);
						}
					}
					else {
						if ((lexTable.table[lexTable.size - 2].lexema[0] != LEX_RIGHTTHESIS
							&& lexTable.table[lexTable.size - 2].lexema[0] != LEX_ID && lexTable.table[lexTable.size - 2].lexema[0] != LEX_LITERAL)
							&& in.text[i - strlen(str)] != ' ' && in.text[i - strlen(str) - 1] == LEX_MINUS) {
							cur_iden.value.vint = (unsigned int)(-stoi(str));
							if (str[0] == '0')
								cur_iden.value.vint = (unsigned int)-stoi(str, nullptr, 8);
							lexTable.size--;
						}
						else {
							cur_iden.value.vint = (unsigned int)(stoi(str));
							if (str[0] == '0')
								cur_iden.value.vint = (unsigned int)stoi(str, nullptr, 8);
						}
					}
					indexIT = IT::search(idTable, cur_iden);
					if (indexIT >= 0) {
						cur_lex.idxTI = indexIT;
						trueFlag = false;
						falseFlag = false;
					}
					else {
						std::sprintf(cur_iden.id, "L%d", number_literal);
						if (trueFlag)
						{
							cur_iden.iddatatype = IT::BOOL;
							cur_iden.value.vint = 1;
							trueFlag = false;
						}
						if (falseFlag)
						{
							cur_iden.iddatatype = IT::BOOL;
							cur_iden.value.vint = 0;
							falseFlag = false;
						}
						cur_iden.idxfirstLE = currentLine;
						cur_iden.scope_name = cur_scope;
						cur_lex.idxTI = idTable.size;
						IT::Add(idTable, cur_iden);
						number_literal++;
					}
				}
				if (cur_lex.lexema[0] == LEX_ID)
				{
					cur_iden.idtype = IT::V;
					cur_iden.scope_name = cur_scope;
					cur_lex.idxTI = idTable.size;
					memcpy(cur_iden.id, str, 15);
					cur_iden.id[15] = '\0';
					cur_iden.iddatatype = IT::UINT;
                                        cur_iden.value.vint = 0.0;
					cur_iden.idxfirstLE = currentLine;
					if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_FUNCTION) {
						if (callFunc) {
							throw ERROR_THROW_IN(600, currentLine, 0);
						}
						callFunc = true;
						cur_iden.idtype = IT::F;
						declareFunc = true;
						if (uintFlag) {
							cur_iden.iddatatype = IT::UINT;
							uintFlag = false;
						}
						if (boolFlag) {
							cur_iden.iddatatype = IT::BOOL;
							boolFlag = false;
						}
						if (doubleFlag) {
							cur_iden.iddatatype = IT::DOUBLE;
							doubleFlag = false;
						}
						if (stringFlag) {
							throw ERROR_THROW_IN(126, currentLine, 0);
							cur_iden.iddatatype = IT::STR;
							stringFlag = false;
						}
						indexIT = IT::search(idTable, cur_iden);
						if (indexIT >= 0) {
							throw ERROR_THROW_IN(107, currentLine, pos);
						}
						if(indexIT == -1){
							lexResult.functions[cur_iden.id] = {};
							cur_lex.idxTI = idTable.size;
							IT::Add(idTable, cur_iden);
							pushToIT = true;
						}
					}
					if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_LEFTTHESIS && lexTable.table[lexTable.size - 3].lexema[0] == LEX_ID
						&& lexTable.table[lexTable.size - 3].idxTI == idTable.size - 1 && idTable.table[idTable.size - 1].idtype == IT::F)
					{
						cur_iden.idtype = IT::P;
						countParms++;
						if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_STRING && stringFlag) {
							cur_iden.iddatatype = IT::STR;
							std::strcpy(cur_iden.value.vstr->str, "");
							stringFlag = false;
						}
						if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_UINTEGER && uintFlag) {
							cur_iden.iddatatype = IT::UINT;
							std::strcpy(cur_iden.value.vstr->str, "");
							uintFlag = false;
						}
						if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_BOOL && boolFlag) {
							cur_iden.iddatatype = IT::BOOL;
							std::strcpy(cur_iden.value.vstr->str, "");
							boolFlag = false;
						}
						if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_DOUBLE && doubleFlag) {
							cur_iden.iddatatype = IT::DOUBLE;
							std::strcpy(cur_iden.value.vstr->str, "");
							doubleFlag = false;
						}
						indexIT = IT::search(idTable, cur_iden);
						if (indexIT >= 0) {
							throw ERROR_THROW_IN(105, currentLine, pos);
						}
						lexResult.functions[cur_scope].push_back(cur_iden.iddatatype);
						cur_lex.idxTI = idTable.size;
						IT::Add(idTable, cur_iden);
						pushToIT = true;
						uintFlag = false;
						boolFlag = false;
						stringFlag = false;
						doubleFlag = false;
					}
					if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_COMMA && idTable.table[lexTable.table[lexTable.size - 3].idxTI].idtype == IT::P) {
						cur_iden.idtype = IT::P;
						countParms++;
						if (countParms > 8) {
							throw ERROR_THROW_IN(108, currentLine, pos);
						}
						if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_STRING && stringFlag) {
							cur_iden.iddatatype = IT::STR;
							std::strcpy(cur_iden.value.vstr->str, "");
							stringFlag = false;
						}
						if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_UINTEGER && uintFlag) {
							cur_iden.iddatatype = IT::UINT;
							std::strcpy(cur_iden.value.vstr->str, "");
							uintFlag = false;
						}
						if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_BOOL && boolFlag) {
							cur_iden.iddatatype = IT::BOOL;
							std::strcpy(cur_iden.value.vstr->str, "");
							boolFlag = false;
						}
						if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_DOUBLE && doubleFlag) {
							cur_iden.iddatatype = IT::DOUBLE;
							std::strcpy(cur_iden.value.vstr->str, "");
							doubleFlag = false;
						}
						indexIT = IT::search(idTable, cur_iden);
						if (indexIT >= 0) {
							throw ERROR_THROW_IN(105, currentLine, pos);
						}
						IT::Add(idTable, cur_iden);
						lexResult.functions[cur_scope].push_back(cur_iden.iddatatype);
						pushToIT = true;
						uintFlag = false;
						boolFlag = false;
						stringFlag = false;
						doubleFlag = false;
					}
					if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_DOUBLE && doubleFlag) {
						cur_iden.iddatatype = IT::DOUBLE;
						std::strcpy(cur_iden.value.vstr->str, "");
						indexIT = IT::search(idTable, cur_iden);
						if (indexIT >= 0) {
							throw ERROR_THROW_IN(105, currentLine, pos);
						}
						IT::Add(idTable, cur_iden);
						pushToIT = true;
						doubleFlag = false;
					}
					if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_UINTEGER && uintFlag) {
						cur_iden.iddatatype = IT::UINT;
						std::strcpy(cur_iden.value.vstr->str, "");
						indexIT = IT::search(idTable, cur_iden);
						if (indexIT >= 0) {
							throw ERROR_THROW_IN(105, currentLine, pos);
						}
						IT::Add(idTable, cur_iden);
						pushToIT = true;
						uintFlag = false;
					}
					if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_BOOL && boolFlag) {
						cur_iden.iddatatype = IT::BOOL;
						std::strcpy(cur_iden.value.vstr->str, "");
						indexIT = IT::search(idTable, cur_iden);
						if (indexIT >= 0) {
							throw ERROR_THROW_IN(105, currentLine, pos);
						}
						IT::Add(idTable, cur_iden);
						pushToIT = true;
						boolFlag = false;
					}
					if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_STRING && stringFlag) {
						cur_iden.iddatatype = IT::STR;
						std::strcpy(cur_iden.value.vstr->str, "");
						indexIT = IT::search(idTable, cur_iden);
						if (indexIT >= 0) {
							throw ERROR_THROW_IN(105, currentLine, pos);
						}
						IT::Add(idTable, cur_iden);
						pushToIT = true;
						stringFlag = false;
					}
					if (!pushToIT) {
						indexIT = IT::search(idTable, cur_iden, scope);
						if (indexIT < 0) {
							throw ERROR_THROW_IN(106, currentLine, pos);
						}
						cur_lex.idxTI = indexIT;
					}
                                        std::memset(cur_iden.id, 0, 15);
					cur_iden.iddatatype = IT::UINT;
                                        cur_iden.value.vint = 0.0;
					pushToIT = false;
				}
				if (cur_lex.lexema[0] == LEX_WHILE) {
					if (lexTable.table[lexTable.size - 1].lexema[0] == TYPE) {
						throw ERROR_THROW_IN(104, currentLine, pos);
					}
					prev_scope = cur_scope;
					cur_scope = UNTIL + to_string(number_until++);
					scope[cur_scope] = prev_scope;
				}
				if (cur_lex.lexema[0] == LEX_IF) {
					if (lexTable.table[lexTable.size - 1].lexema[0] == TYPE) {
						throw ERROR_THROW_IN(104, currentLine, pos);
					}
					prev_scope = cur_scope;
					cur_scope = IF + to_string(number_if++);
					scope[cur_scope] = prev_scope;
				}
				if (cur_lex.lexema[0] == LEX_ELSE) {
					if (lexTable.table[lexTable.size - 1].lexema[0] == TYPE) {
						throw ERROR_THROW_IN(104, currentLine, pos);
					}
					prev_scope = cur_scope;
					cur_scope = ELSE + to_string(number_else++);
					scope[cur_scope] = prev_scope;
				}
				if (lexTable.table[lexTable.size - 1].lexema[0] == TYPE && cur_lex.lexema[0] != LEX_ID) {
					throw ERROR_THROW_IN(104, currentLine, pos);
				}
				if (in.text[i] != MARK) {
					bufferIndex = 0;
                                        std::memset(str, 0, bufferIndex + 1);
				}
			}
                        if (cur_lex.lexema[0] != '\0')
			{
				cur_lex.sn = currentLine;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
			}
			if (litflag && in.text[i] == MARK && bufferIndex == 1) {
				continue;
			}
			else if (litflag && in.text[i] != MARK) {
				if (in.text[i] == '`') {
					throw ERROR_THROW_IN(125, currentLine, pos);
				}
				continue;
			}
			switch (in.text[i])
			{
			case MARK:
				litflag = true;
				if (str[0] == MARK)
				{
					cur_lex.idxTI = idTable.size;
					str[bufferIndex] = MARK;
					bufferIndex++;
					str[bufferIndex] = '\0';
					litflag = false;
					cur_lex.lexema[0] = LEX_LITERAL;
					std::sprintf(cur_iden.id, "L%d", number_literal);
					number_literal++;
					cur_iden.iddatatype = IT::STR;
					cur_iden.idtype = IT::L;
					cur_iden.idxfirstLE = currentLine;
					for (int i = 0; i < strlen(str); i++)
					{
						cur_iden.value.vstr->str[i] = str[i];
					}
					cur_iden.value.vstr->str[strlen(str)] = '\0';
					cur_iden.value.vstr->len = strlen(cur_iden.value.vstr->str);
					cur_lex.sn = currentLine;
					cur_iden.scope_name = cur_scope;
					LT::Add(lexTable, cur_lex);
					IT::Add(idTable, cur_iden);
					bufferIndex = 0;
                                        std::memset(str, 0, bufferIndex + 1);
					cur_lex.lexema[0] = '\0';
					break;
				}
				break;
			case NEW_LINE:
				currentLine++;
				cur_lex.lexema[0] = '\0';
				pos = 0;
				break;
			case LEX_SEMICOLON:
				cur_lex.lexema[0] = LEX_SEMICOLON;
				if (in.text[i - 1] == NEW_LINE) {
					throw ERROR_THROW_IN(602, currentLine, 0);
				}
				cur_lex.sn = currentLine;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
				if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_BRACELET)
				{
					if (cur_scope.find(UNTIL) != -1) {
						whileflag = false;
					}
					else if (cur_scope.find(IF) != -1) {
						ifflag = false;
					}
					else if (cur_scope.find(ELSE) != -1) {
						elseflag = false;
					}
					else {
						declareFunc = false;
						callFunc = false;
					}
					cur_scope = prev_scope;
					prev_scope = scope[cur_scope];
				}
				break;
			case LEX_COMMA:
				cur_lex.lexema[0] = LEX_COMMA;
				cur_lex.sn = currentLine;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
				break;
			case LEX_LEFTBRACE:
				if (mainFlag)
				{
					scope["main"] = cur_scope;
					prev_scope = cur_scope;
					cur_scope = "main";
					mainFlag = false;
				}
				cur_lex.lexema[0] = LEX_LEFTBRACE;
				cur_lex.sn = currentLine;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
				break;
			case LEX_BRACELET:
				cur_lex.lexema[0] = LEX_BRACELET;
				if (in.text[i + 1] != ';') {
					throw ERROR_THROW_IN(601, currentLine, 0);
				}
				cur_lex.sn = currentLine;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
				break;
			case LEX_LEFTTHESIS:
				cur_lex.lexema[0] = LEX_LEFTTHESIS;
				cur_lex.sn = currentLine;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
				if (whileflag || ifflag) {
					continue;
				}
				else if (declareFunc)
				{
					parmFlag = true;
					for (int i = idTable.size - 1; i >= 0; i--) {
						if (idTable.table[i].idtype == IT::F) {
							scope[idTable.table[i].id] = cur_scope;
							prev_scope = cur_scope;
							cur_scope = idTable.table[i].id;
							countParms = 0;
							break;
						}
					}
				}
				break;
			case LEX_RIGHTTHESIS:
				cur_lex.lexema[0] = LEX_RIGHTTHESIS;
				cur_lex.sn = currentLine;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
				parmFlag = false;
				break;
			case LEX_PLUS:
				cur_lex.lexema[0] = LEX_PLUS;
				cur_lex.sn = currentLine;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
				break;
			case LEX_MINUS:
				cur_lex.lexema[0] = LEX_MINUS;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
				break;
			case LEX_STAR:
				cur_lex.lexema[0] = LEX_STAR;
				cur_lex.sn = currentLine;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
				break;
			case LEX_DIRSLASH:
				cur_lex.lexema[0] = LEX_DIRSLASH;
				cur_lex.sn = currentLine;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
				break;
			case LEX_EQUAL:
				if (in.text[i + 1] == LEX_EQUAL && in.text[i - 1] != LEX_EQUAL) {
					cur_lex.lexema[0] = LEX_ISEQUAL;
					i++;
					cur_lex.sn = currentLine;
					LT::Add(lexTable, cur_lex);
					cur_lex.lexema[0] = '\0';
					break;
				}
				if (in.text[i - 1] != LEX_EQUAL && in.text[i + 1] != LEX_EQUAL) {
					cur_lex.lexema[0] = LEX_EQUAL;
					// if (lexTable.table[lexTable.size - 1].lexema[0] != LEX_ID) {
					// 	throw ERROR_THROW_IN(602, currentLine, pos);
					// }
					cur_lex.sn = currentLine;
					LT::Add(lexTable, cur_lex);
					cur_lex.lexema[0] = '\0';
					break;
				}
				else {
					throw ERROR_THROW_IN(601, currentLine, pos);
				}
			case LEX_NOT:
				if (in.text[i + 1] == LEX_EQUAL) {
					cur_lex.lexema[0] = LEX_NOT_EQUAL;
					i++;
				}
				else {
					throw ERROR_THROW_IN(601, currentLine, pos);
				}
				cur_lex.sn = currentLine;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
				break;
			case LEX_MORE:
				cur_lex.lexema[0] = LEX_MORE;
				if (in.text[i + 1] == EQUAL) {
					cur_lex.lexema[0] = LEX_MORE_OR_EQUAL;
					i++;
				}
				cur_lex.sn = currentLine;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
				break;
			case LEX_LESS:
				cur_lex.lexema[0] = LEX_LESS;
				if (in.text[i + 1] == EQUAL) {
					cur_lex.lexema[0] = LEX_LESS_OR_EQUAL;
					i++;
				}
				cur_lex.sn = currentLine;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
				break;
			case LEX_OST:
				cur_lex.lexema[0] = LEX_OST;
				cur_lex.sn = currentLine;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
				break;
			case LEX_AND:
				cur_lex.lexema[0] = LEX_AND;
				cur_lex.sn = currentLine;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
				break;
			case LEX_OR:
				cur_lex.lexema[0] = LEX_OR;
				cur_lex.sn = currentLine;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
				break;
			}
		}
		currentLine = 1;
		LT_file << currentLine;
		LT_file << '\t';
		for (int i = 0; i < lexTable.size; i++)
		{
			cur_lex = LT::GetEntry(lexTable, i);
			if (currentLine != cur_lex.sn)
			{
				currentLine = cur_lex.sn;
				LT_file << '\n';
				LT_file << currentLine;
				LT_file << '\t';
			}
			LT_file << cur_lex.lexema[0];
		}
		LT_file.close();
		IT_file << std::setw(10) << "id"
			<< std::setw(20) << "datatype"
			<< std::setw(20) << "idtype"
			<< std::setw(20) << "Line"
			<< std::setw(20) << "value"
			<< std::setw(20) << "Scope"
		  << std::setw(20) << "idTi" << std::endl;

		for (int i = 0; i < idTable.size; i++) {
			cur_iden = IT::GetEntry(idTable, i);
			IT_file << std::setw(10) << cur_iden.id;
			if (cur_iden.iddatatype == 1)
				IT_file << std::setw(20) << "UINT";
			if (cur_iden.iddatatype == 2)
				IT_file << std::setw(20) << "STR";
			if (cur_iden.iddatatype == 3)
				IT_file << std::setw(20) << "BOOL";
			if (cur_iden.iddatatype == 4)
				IT_file << std::setw(20) << "DOUBLE";
			if (cur_iden.idtype == IT::V)
				IT_file << std::setw(20) << "V";
			if (cur_iden.idtype == IT::L)
				IT_file << std::setw(20) << "L";
			if (cur_iden.idtype == IT::F)
				IT_file << std::setw(20) << "F";
			if (cur_iden.idtype == IT::P)
				IT_file << std::setw(20) << "P";
			IT_file << std::setw(20) << cur_iden.idxfirstLE;

			if ((cur_iden.iddatatype == IT::UINT || cur_iden.iddatatype == IT::DOUBLE || cur_iden.iddatatype == IT::BOOL) && cur_iden.idtype == IT::L) {
				IT_file << std::setw(20) << cur_iden.value.vint;
			}
			if ((cur_iden.iddatatype == IT::UINT || cur_iden.iddatatype == IT::DOUBLE || cur_iden.iddatatype == IT::BOOL || cur_iden.iddatatype == IT::STR) && cur_iden.idtype != IT::L) {
				IT_file << std::setw(20) << ' ';
			}
			if (cur_iden.iddatatype == IT::STR && cur_iden.idtype == IT::L) {
				IT_file << std::setw(16);
				for (int j = 0; j < strlen(cur_iden.value.vstr->str); j++) {
					IT_file << cur_iden.value.vstr->str[j];
				}
				IT_file << std::setw(20);
			}
			IT_file << std::setw(20) << cur_iden.scope_name;
			IT_file << std::setw(20);
			IT_file << cur_iden.idxfirstLE;
			IT_file << std::endl;
		}
		IT_file.close();
		lexResult.idTable = idTable;
		lexResult.lexTable = lexTable;
		return lexResult;
	}
}