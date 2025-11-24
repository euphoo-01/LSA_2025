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
	bool whileflag = false; // Для because
	bool ifflag = false;
	bool differflag = false; // Для differ
	bool unsignedKeywordFlag = false; // Для "unsigned"
	bool integerKeywordFlag = false; // Для "integer"
	bool charTypeFlag = false; // Для "char" type
	bool logicTypeFlag = false; // Для "logic" type
	bool parmFlag = false;
	bool litflag = false;
	// bool falseFlag = false; // LSA-2025 boolean literals are 0 or 1, not keywords
	// bool trueFlag = false; // LSA-2025 boolean literals are 0 or 1, not keywords
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
		FST_UNSIGNED
		FST_INTEGER
		FST_CHAR_TYPE_KEYWORD
		FST_LOGIC_TYPE_KEYWORD
		FST_FUNC_KEYWORD
		FST_SEND
		FST_BECAUSE
		FST_DIFFER
		FST_MAIN
		FST_IF
		FST_LITERAL
		FST_IDEN

		if (FST::execute(_unsigned)) {
			unsignedKeywordFlag = true;
			return '\0';
		}
		if (FST::execute(_integer)) {
			integerKeywordFlag = true;
			return '\0';
		}
		if (FST::execute(_char_type_keyword)) {
			charTypeFlag = true;
			return LEX_CHAR_TYPE;
		}
		if (FST::execute(_logic_type_keyword)) {
			logicTypeFlag = true;
			return LEX_LOGIC_TYPE;
		}
		if (FST::execute(_func_keyword)) {
			return LEX_FUNC;
		}
		if (FST::execute(_send)) {
			return LEX_SEND;
		}
		if (FST::execute(_because)) {
			whileflag = true; // Переименовать для ясности, относится к "because"
			return LEX_BECAUSE;
		}
		if (FST::execute(_differ)) {
			differflag = true;
			return LEX_DIFFER;
		}
		if (FST::execute(_main)) {
			mainFlag = true;
			return LEX_MAIN;
		}
		if (FST::execute(_if)) {
			ifflag = true;
			return LEX_IF;
		}
		if (FST::execute(_literal)) {
			return LEX_LITERAL;
		}
		if (FST::execute(_iden)) {
			return LEX_ID;
		}
		return '\0';
	}
	LEX LexA(Parm::PARm parm, In::iN in)
	{
		LEX lexResult;
		IT::Add(idTable, *new IT::Entry(-1, "compare", IT::LOGIC, IT::F, "global", 2));
		IT::Add(idTable, *new IT::Entry(-1, "len", IT::UINT, IT::F, "global", 1));
		IT::Add(idTable, *new IT::Entry(-1, "getmin", IT::UINT, IT::F, "global", 2));
		IT::Add(idTable, *new IT::Entry(-1, "getmax", IT::UINT, IT::F, "global", 2));
		lexResult.functions["compare"].push_back(IT::CHAR);
		lexResult.functions["compare"].push_back(IT::CHAR);
		lexResult.functions["getmin"].push_back(IT::UINT);
		lexResult.functions["getmin"].push_back(IT::UINT);
		lexResult.functions["getmax"].push_back(IT::UINT);
		lexResult.functions["getmax"].push_back(IT::UINT);
		lexResult.functions["len"].push_back(IT::CHAR);
		int indexIT;
		int pos = 0;
		LT::Entry cur_lex;
		int bufferIndex = 0;
		cur_lex.sn = 0;
		cur_lex.idxTI = 0;
		cur_lex.lexema[0] = '\0';
		int number_literal = 0;
		int number_because = 0; // Renamed from number_until
		int number_if = 0;
		int number_differ = 0; // Renamed from number_else
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
					cur_lex.lexema[0] = FST(); // Call FST to get the base lexeme
				} else {
					cur_lex.lexema[0] = '\0';
				}

				// Special handling for "unsigned integer"
				if (unsignedKeywordFlag) {
					if (integerKeywordFlag) {
						cur_lex.lexema[0] = LEX_UINT_TYPE;
						unsignedKeywordFlag = false;
						integerKeywordFlag = false;
					} else { // "unsigned" was detected, but "integer" was not the next keyword
						// This indicates an error or "unsigned" used alone, which is invalid
						// For now, treat as invalid keyword sequence, FST already returned '\0'
						// If FST returned '\0', cur_lex.lexema[0] is already '\0' so it will be ignored later.
						// The unsignedKeywordFlag should be reset immediately if not followed by 'integer'.
						// Re-lex 'unsigned' as an identifier or throw an error.
						// For now, assume a correct 'unsigned integer' sequence or it's an error handled later
						unsignedKeywordFlag = false; // Reset flag
					}
				} else if (integerKeywordFlag) { // 'integer' was found without 'unsigned' before it.
					// Also an error, 'integer' is not a standalone type keyword.
					integerKeywordFlag = false; // Reset flag
				}
				
				// Handle single-character tokens (if bufferIndex was 0)
				// The FST() result takes precedence if bufferIndex > 0.
				// If bufferIndex was 0, it means we are dealing with a separator or operator.
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
							if (in.text[i + 1] == LEX_PLUS) { // ++ operator
								cur_lex.lexema[0] = LEX_INCREMENT;
								i++; // Consume next character
							} else {
								cur_lex.lexema[0] = LEX_PLUS;
							}
							break;
						case LEX_MINUS:
							if (in.text[i + 1] == LEX_MINUS) { // -- operator
								cur_lex.lexema[0] = LEX_DECREMENT;
								i++; // Consume next character
							} else {
								cur_lex.lexema[0] = LEX_MINUS;
							}
							break;
						case LEX_STAR:
							cur_lex.lexema[0] = LEX_STAR;
							break;
						case LEX_COLON: // New division operator
							cur_lex.lexema[0] = LEX_COLON;
							break;
						case LEX_EQUAL:
							if (in.text[i + 1] == LEX_EQUAL) { // == operator
								cur_lex.lexema[0] = LEX_ISEQUAL;
								i++; // Consume next character
							} else {
								cur_lex.lexema[0] = LEX_EQUAL; // Assignment operator
							}
							break;
						case LEX_MORE:
							if (in.text[i + 1] == LEX_EQUAL) { // >= operator
								cur_lex.lexema[0] = LEX_MORE_OR_EQUAL;
								i++; // Consume next character
							} else {
								cur_lex.lexema[0] = LEX_MORE;
							}
							break;
						case LEX_LESS:
							if (in.text[i + 1] == LEX_EQUAL) { // <= operator
								cur_lex.lexema[0] = LEX_LESS_OR_EQUAL;
								i++; // Consume next character
							} else {
								cur_lex.lexema[0] = LEX_LESS;
							}
							break;
						case LEX_BITNOT: // ~ operator
							cur_lex.lexema[0] = LEX_BITNOT;
							break;
						case '!': // Only for != (not equal)
							if (in.text[i + 1] == LEX_EQUAL) {
								cur_lex.lexema[0] = LEX_NOT_EQUAL;
								i++; // Consume next character
							} else {
								throw ERROR_THROW_IN(601, currentLine, pos); // Invalid operator '!'
							}
							break;
						default:
							cur_lex.lexema[0] = '\0';
							break;
					}
				}

				// Processing recognized lexeme
				if (cur_lex.lexema[0] == LEX_MAIN)
				{
					if (callFunc) {
						throw ERROR_THROW_IN(600, currentLine, 0);
					}
					mainFlag = true;
					cur_lex.idxTI = idTable.size;
					memcpy(cur_iden.id, str, 15);
					cur_iden.id[15] = '\0';
					cur_iden.iddatatype = IT::UINT; // main returns unsigned integer
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
				else if (cur_lex.lexema[0] == LEX_LITERAL)
				{
					cur_iden.idtype = IT::L;
					std::sprintf(cur_iden.id, ""); // Reset id for literal
					bool dot = false;
					// Check if literal is a double (not allowed in LSA-2025 except by conversion) or has a decimal point
					for (int j = 0; j < strlen(str); j++) {
						if (str[j] == '.') {
							dot = true;
							break;
						}
					}

					// LSA-2025 only has unsigned integer, char, logic literals
					// Integer literals
					if (!dot) {
						cur_iden.iddatatype = IT::UINT;
						// Handle signed literals temporarily for parsing, then ensure unsigned
						long long val_ll = stoll(str);
						if (val_ll < 0 || val_ll > 4294967295ULL) { // Check against unsigned integer max value
							throw ERROR_THROW_IN(120, currentLine, pos); // Value out of range
						}
						cur_iden.value.vint = static_cast<double>(val_ll); // Store as double, convert to uint later
						if (str[0] == '0' && strlen(str) > 1) { // Octal literal handling, if spec supports
							cur_iden.value.vint = static_cast<double>(stoll(str, nullptr, 8));
						}
					}
					// Char literals (single character within marks) are handled in the MARK case below.
					// If a literal that passed FST_LITERAL, and it's not an integer, it's an error.
					else {
						throw ERROR_THROW_IN(120, currentLine, pos); // LSA-2025 does not support float/double literals
					}

					indexIT = IT::search(idTable, cur_iden);
					if (indexIT >= 0) {
						cur_lex.idxTI = indexIT;
					} else {
						std::sprintf(cur_iden.id, "L%d", number_literal);
						cur_iden.idxfirstLE = currentLine;
						cur_iden.scope_name = cur_scope;
						cur_lex.idxTI = idTable.size;
						IT::Add(idTable, cur_iden);
						number_literal++;
					}
				}
				else if (cur_lex.lexema[0] == LEX_ID)
				{
					cur_iden.idtype = IT::V;
					cur_iden.scope_name = cur_scope;
					cur_lex.idxTI = idTable.size;
					memcpy(cur_iden.id, str, 15);
					cur_iden.id[15] = '\0';
					cur_iden.iddatatype = IT::UINT; // Default to UINT, will be updated by type keywords
                    cur_iden.value.vint = 0.0;
					cur_iden.idxfirstLE = currentLine;

					// Check if previous token was a type keyword for declaration
					if (lexTable.size > 0) {
						char prevLex = lexTable.table[lexTable.size - 1].lexema[0];
						if (prevLex == LEX_FUNC) {
							if (callFunc) {
								throw ERROR_THROW_IN(600, currentLine, 0); // Nested func calls, invalid syntax
							}
							callFunc = true;
							cur_iden.idtype = IT::F;
							declareFunc = true;

							// Determine function return type based on previous type keyword
							if (charTypeFlag) {
								cur_iden.iddatatype = IT::CHAR;
								charTypeFlag = false;
							} else if (logicTypeFlag) {
								cur_iden.iddatatype = IT::LOGIC;
								logicTypeFlag = false;
							} else if (unsignedKeywordFlag && integerKeywordFlag) { // "unsigned integer"
								cur_iden.iddatatype = IT::UINT;
								unsignedKeywordFlag = false;
								integerKeywordFlag = false;
							} else {
								// Default to UINT or throw error if func declaration type is missing/invalid
								throw ERROR_THROW_IN(120, currentLine, pos); // Invalid type for function
							}

							indexIT = IT::search(idTable, cur_iden);
							if (indexIT >= 0) {
								throw ERROR_THROW_IN(107, currentLine, pos); // Function already defined
							}
							if(indexIT == -1){
								lexResult.functions[cur_iden.id] = {};
								cur_lex.idxTI = idTable.size;
								IT::Add(idTable, cur_iden);
								pushToIT = true;
							}
						}
						// Parameter declaration
						else if (prevLex == LEX_LEFTTHESIS || prevLex == LEX_COMMA) {
							// Look further back to see if it's a function parameter
							bool is_param = false;
							if (lexTable.size >= 2 && idTable.table[lexTable.table[lexTable.size - (prevLex == LEX_LEFTTHESIS ? 2 : 3)].idxTI].idtype == IT::F) {
								is_param = true;
							}
							if (is_param && declareFunc) { // Only if currently declaring a function
								cur_iden.idtype = IT::P;
								countParms++;
								if (countParms > 8) { // Max parameters, as per previous code
									throw ERROR_THROW_IN(108, currentLine, pos);
								}

								// Determine parameter type based on previous type keyword
								if (charTypeFlag) {
									cur_iden.iddatatype = IT::CHAR;
									charTypeFlag = false;
								} else if (logicTypeFlag) {
									cur_iden.iddatatype = IT::LOGIC;
									logicTypeFlag = false;
								} else if (unsignedKeywordFlag && integerKeywordFlag) { // "unsigned integer"
									cur_iden.iddatatype = IT::UINT;
									unsignedKeywordFlag = false;
									integerKeywordFlag = false;
								} else {
									throw ERROR_THROW_IN(120, currentLine, pos); // Invalid type for parameter
								}

								indexIT = IT::search(idTable, cur_iden);
								if (indexIT >= 0) {
									throw ERROR_THROW_IN(105, currentLine, pos); // Duplicate identifier
								}
								lexResult.functions[cur_scope].push_back(cur_iden.iddatatype);
								cur_lex.idxTI = idTable.size;
								IT::Add(idTable, cur_iden);
								pushToIT = true;
							}
						}
						// Variable declaration
						else if (charTypeFlag || logicTypeFlag || (unsignedKeywordFlag && integerKeywordFlag)) {
							cur_iden.idtype = IT::V;
							if (charTypeFlag) {
								cur_iden.iddatatype = IT::CHAR;
								charTypeFlag = false;
							} else if (logicTypeFlag) {
								cur_iden.iddatatype = IT::LOGIC;
								logicTypeFlag = false;
							} else if (unsignedKeywordFlag && integerKeywordFlag) {
								cur_iden.iddatatype = IT::UINT;
								unsignedKeywordFlag = false;
								integerKeywordFlag = false;
							}

							indexIT = IT::search(idTable, cur_iden);
							if (indexIT >= 0) {
								throw ERROR_THROW_IN(105, currentLine, pos); // Duplicate identifier
							}
							cur_lex.idxTI = idTable.size;
							IT::Add(idTable, cur_iden);
							pushToIT = true;
						}
					}

					// If not a declaration, search for existing identifier
					if (!pushToIT) {
						indexIT = IT::search(idTable, cur_iden, scope);
						if (indexIT < 0) {
							throw ERROR_THROW_IN(106, currentLine, pos); // Identifier not defined
						}
						cur_lex.idxTI = indexIT;
					}
                    std::memset(cur_iden.id, 0, 15);
					cur_iden.iddatatype = IT::UINT; // Reset to default
                    cur_iden.value.vint = 0.0; // Reset
					pushToIT = false;
				}
				
				// Keywords (char, logic, unsigned, integer) that are *not* part of "unsigned integer" combination
				else if (charTypeFlag && cur_lex.lexema[0] == LEX_ID) {
					// This case should be handled by the declaration logic above.
					// This branch might be redundant or indicate an issue if reached.
					charTypeFlag = false;
				}
				else if (logicTypeFlag && cur_lex.lexema[0] == LEX_ID) {
					logicTypeFlag = false;
				}


				if (cur_lex.lexema[0] == LEX_BECAUSE) {
					prev_scope = cur_scope;
					cur_scope = UNTIL_SCOPE + to_string(number_because++); // Using UNTIL_SCOPE string literal from LexA.h
					scope[cur_scope] = prev_scope;
				}
				if (cur_lex.lexema[0] == LEX_IF) {
					prev_scope = cur_scope;
					cur_scope = IF_SCOPE + to_string(number_if++); // Using IF_SCOPE string literal from LexA.h
					scope[cur_scope] = prev_scope;
				}
				if (cur_lex.lexema[0] == LEX_DIFFER) {
					prev_scope = cur_scope;
					cur_scope = ELSE_SCOPE + to_string(number_differ++); // Using ELSE_SCOPE string literal from LexA.h
					scope[cur_scope] = prev_scope;
				}
				// if (lexTable.table[lexTable.size - 1].lexema[0] == TYPE && cur_lex.lexema[0] != LEX_ID) { // TYPE is removed
				// 	throw ERROR_THROW_IN(104, currentLine, pos);
				// }
				if (in.text[i] != MARK) {
					bufferIndex = 0;
                    std::memset(str, 0, bufferIndex + 1);
				}
			}
            if (cur_lex.lexema[0] != '\0') // Only add if a valid lexeme was found
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
				if (str[0] == MARK) // If the mark was the first character in str
				{
					// This block handles string literals like 'abc'
					cur_lex.idxTI = idTable.size;
					str[bufferIndex] = MARK; // Add closing mark
					bufferIndex++;
					str[bufferIndex] = '\0';
					litflag = false;
					cur_lex.lexema[0] = LEX_LITERAL;
					std::sprintf(cur_iden.id, "L%d", number_literal);
					number_literal++;
					cur_iden.iddatatype = IT::CHAR; // LSA-2025 char literals are single char
					cur_iden.idtype = IT::L;
					cur_iden.idxfirstLE = currentLine;
					// Ensure str length is 1 for char literal
					if (strlen(str) != 3 || (strlen(str) == 3 && str[1] == '\\' && str[2] != 'n')) { // 'c' or '\n'
						// Special handling for '\n'
						if (strcmp(str, "'\\n'") != 0)
							throw ERROR_THROW_IN(120, currentLine, pos); // Invalid char literal format
					}

					// Copy char literal content. For LSA-2025, it's a single char.
					// The vstr structure might need adjustment for single char.
					// For now, assume a single char.
					// The value.vstr is a char array inside a union. It's used for strings,
					// so a single char is effectively a string of length 1.
					cur_iden.value.vstr->str[0] = str[1]; // Get the character itself (e.g. 'A' from "'A'")
					cur_iden.value.vstr->str[1] = '\0';
					cur_iden.value.vstr->len = 1;

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
					if (cur_scope.find(UNTIL_SCOPE) != -1) { // Renamed
						whileflag = false; // Flag for 'because' loop
					}
					else if (cur_scope.find(IF_SCOPE) != -1) { // Renamed
						ifflag = false;
					}
					else if (cur_scope.find(ELSE_SCOPE) != -1) { // Renamed for 'differ'
						differflag = false; // Flag for 'differ'
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
				if (whileflag || ifflag) { // Flags for 'because' or 'if'
					// Keep behavior for now, might need refinement
					continue;
				}
				else if (declareFunc)
				{
					parmFlag = true;
					for (int i_it = idTable.size - 1; i_it >= 0; i_it--) {
						if (idTable.table[i_it].idtype == IT::F) {
							scope[idTable.table[i_it].id] = cur_scope;
							prev_scope = cur_scope;
							cur_scope = idTable.table[i_it].id;
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
				if (in.text[i + 1] == LEX_PLUS) { // ++ operator
					cur_lex.lexema[0] = LEX_INCREMENT;
					i++;
				} else {
					cur_lex.lexema[0] = LEX_PLUS;
				}
				cur_lex.sn = currentLine;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
				break;
			case LEX_MINUS:
				if (in.text[i + 1] == LEX_MINUS) { // -- operator
					cur_lex.lexema[0] = LEX_DECREMENT;
					i++;
				} else {
					cur_lex.lexema[0] = LEX_MINUS;
				}
				cur_lex.sn = currentLine;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
				break;
			case LEX_STAR:
				cur_lex.lexema[0] = LEX_STAR;
				cur_lex.sn = currentLine;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
				break;
			case LEX_COLON: // New division operator
				cur_lex.lexema[0] = LEX_COLON;
				cur_lex.sn = currentLine;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
				break;
			case LEX_EQUAL:
				if (in.text[i + 1] == LEX_EQUAL) { // == operator
					cur_lex.lexema[0] = LEX_ISEQUAL;
					i++;
				} else {
					cur_lex.lexema[0] = LEX_EQUAL; // Assignment operator
				}
				cur_lex.sn = currentLine;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
				break;
			case LEX_MORE:
				if (in.text[i + 1] == LEX_EQUAL) { // >= operator
					cur_lex.lexema[0] = LEX_MORE_OR_EQUAL;
					i++;
				} else {
					cur_lex.lexema[0] = LEX_MORE;
				}
				cur_lex.sn = currentLine;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
				break;
			case LEX_LESS:
				if (in.text[i + 1] == LEX_EQUAL) { // <= operator
					cur_lex.lexema[0] = LEX_LESS_OR_EQUAL;
					i++;
				} else {
					cur_lex.lexema[0] = LEX_LESS;
				}
				cur_lex.sn = currentLine;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
				break;
			case LEX_BITNOT: // ~ operator
				cur_lex.lexema[0] = LEX_BITNOT;
				cur_lex.sn = currentLine;
				LT::Add(lexTable, cur_lex);
				cur_lex.lexema[0] = '\0';
				break;
			case '!': // Only for != (not equal)
				if (in.text[i + 1] == LEX_EQUAL) {
					cur_lex.lexema[0] = LEX_NOT_EQUAL;
					i++;
				} else {
					throw ERROR_THROW_IN(601, currentLine, pos); // Invalid operator '!'
				}
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
			if (cur_iden.iddatatype == IT::UINT)
				IT_file << std::setw(20) << "UINT";
			if (cur_iden.iddatatype == IT::CHAR) // Changed
				IT_file << std::setw(20) << "CHAR"; // Changed
			if (cur_iden.iddatatype == IT::LOGIC) // Changed
				IT_file << std::setw(20) << "LOGIC"; // Changed
			
			if (cur_iden.idtype == IT::V)
				IT_file << std::setw(20) << "V";
			if (cur_iden.idtype == IT::L)
				IT_file << std::setw(20) << "L";
			if (cur_iden.idtype == IT::F)
				IT_file << std::setw(20) << "F";
			if (cur_iden.idtype == IT::P)
				IT_file << std::setw(20) << "P";
			IT_file << std::setw(20) << cur_iden.idxfirstLE;

			if ((cur_iden.iddatatype == IT::UINT || cur_iden.iddatatype == IT::LOGIC) && cur_iden.idtype == IT::L) { // Changed
				IT_file << std::setw(20) << cur_iden.value.vint;
			}
			if ((cur_iden.iddatatype == IT::UINT || cur_iden.iddatatype == IT::LOGIC || cur_iden.iddatatype == IT::CHAR) && cur_iden.idtype != IT::L) { // Changed
				IT_file << std::setw(20) << ' ';
			}
			if (cur_iden.iddatatype == IT::CHAR && cur_iden.idtype == IT::L) { // Changed
				IT_file << std::setw(16);
				for (int j = 0; j < strlen(cur_iden.value.vstr->str); j++) {
					IT_file << cur_iden.value.vstr->str[j]);
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