#include"Lex.h"

#include <cstring>

#include"stdafx.h"
#include"In.h"
#include"Error.h"
#include <stdio.h>
#include <string.h>
#include <iomanip>
#include <vector>
#include <map>

namespace Lex {
    LT::LexTable lexTable = LT::Create(LT_MAXSIZE - 1);
    IT::IdTable idTable = IT::Create(TI_MAXSIZE - 1);
    int countParms = 0;
    bool becauseFlag = false;
    bool ifFlag = false;
    bool differFlag = false;
    bool charFlag = false;
    bool unsignedFlag = false;
    bool intFlag = false;
    bool logicFlag = false;
    bool parmFlag = false;

    bool falseFlag = false;
    bool trueFlag = false;
    bool mainFlag = false;
    bool callFunc;
    char *str = new char[TI_MAXSIZE];
    bool declareFlag = false;
    bool declareFunc = false;
    bool pushToIT = false;

    void Add_LT(int line, char token) {
        LT::Entry entry;
        entry.sn = line;
        memset(entry.lexema, '\0', strlen(entry.lexema) - 1);
        entry.lexema[0] = token;
        LT::Add(lexTable, entry);
    }

    char FST() {
        FST_UNSIGNED
        FST_INTEGER
        FST_CHAR
        FST_LOGIC
        FST_FUNC
        FST_SEND
        FST_MAIN
        FST_WRITECH
        FST_READCH
        FST_TRUE
        FST_FALSE
        FST_BECAUSE
        FST_IF
        FST_LITERAL
        FST_IDEN
        FST_DIFFER
        if (FST::execute(_unsigned)) {
            unsignedFlag = true;
            return NULL; // Incomplete type, continue tokenizing
        }
        if (FST::execute(_integer)) {
            if (unsignedFlag) {
                intFlag = true;
                unsignedFlag = false; // Reset flag after use
                return LEX_INTEGER;
            } else {
                throw ERROR_THROW_IN(127, 0, 0); // 'integer' without 'unsigned' is not a valid type
            }
        }
        if (FST::execute(_logic)) {
            logicFlag = true;
            unsignedFlag = false;
            return LEX_LOGIC;
        }
        if (FST::execute(_char)) {
            charFlag = true;
            unsignedFlag = false;
            return LEX_CHAR;
        }

        // For any other token, reset the unsigned flag
        unsignedFlag = false;

        if (FST::execute(_func)) {
            return LEX_FUNC;
        }
        if (FST::execute(_send))
            return LEX_SEND;
        if (FST::execute(_main)) {
            mainFlag = true;
            return LEX_MAIN;
        }
        if (FST::execute(_writech))
            return LEX_WRITECH;
        if (FST::execute(_readch))
            return LEX_READCH;
        if (FST::execute(_true)) {
            trueFlag = true;
            return LEX_LITERAL;
        }
        if (FST::execute(_false)) {
            falseFlag = true;
            return LEX_LITERAL;
        }
        if (FST::execute(_because)) {
            becauseFlag = true;
            return LEX_BECAUSE;
        }
        if (FST::execute(_if)) {
            ifFlag = true;
            return LEX_IF;
        }
        if (FST::execute(_differ)) {
            differFlag = true;
            return LEX_DIFFER;
        }
        if (FST::execute(_literal))
            return LEX_LITERAL;
        if (FST::execute(_iden))
            return LEX_ID;
        return NULL;
    }

    LEX LexA(Parm::Parm parm, In::IN in) {
        LEX lexResult;
        // IT::Add(idTable, *new IT::Entry(-1, "compare", IT::UINT, IT::F, "global", 2));
        // lexResult.functions["compare"].push_back(IT::STR);
        // lexResult.functions["compare"].push_back(IT::STR);
        int indexIT;
        int pos = 0;
        LT::Entry cur_lex;
        int bufferIndex = 0;
        cur_lex.sn = 0;
        cur_lex.idxTI = 0;
        cur_lex.lexema[0] = NULL;
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
        for (int i = 0; i < in.size; i++) {
            pos++;
            if (((in.text[i] >= 'A' && in.text[i] <= 'Z') || (in.text[i] >= 'a' && in.text[i] <= 'z') || (
                     in.text[i] >= '0' && in.text[i] <= '9') || (in.text[i] >= 0xC0 && in.text[i] <= 0xFF) || (
                     in.text[i] >= 0xE0 && in.text[i] <= 0xFF) || in.text[i] == DOT) && in.text[i] != SPACE) {
                str[bufferIndex] = in.text[i];
                bufferIndex++;
                if (bufferIndex >= TI_MAXSIZE) {
                    throw ERROR_THROW(119);
                }
            } else { // This block processes token if not part of identifier/number
                str[bufferIndex] = '\0'; // Null-terminate accumulated string

                if (cur_lex.lexema[0] == NULL) { // Only call FST if a token hasn't been identified yet
                    cur_lex.lexema[0] = FST();
                }
                if (cur_lex.lexema[0] == LEX_MAIN) {
                    if (callFunc) {
                        throw ERROR_THROW_IN(600, currentLine, 0);
                    }
                    mainFlag = true;
                    cur_lex.idxTI = idTable.size;
                    memcpy(cur_iden.id, str, 15);
                    cur_iden.id[15] = '\0';
                    cur_iden.iddatatype = IT::UNSIGNED;
                    cur_iden.idtype = IT::F;
                    cur_iden.value.vint = NULL;
                    cur_iden.idxfirstLE = currentLine;
                    cur_iden.scope_name = cur_scope;
                    indexIT = IT::search(idTable, cur_iden);
                    if (indexIT >= 0) {
                        throw ERROR_THROW_IN(109, currentLine, pos);
                    }
                    if (indexIT == -1) {
                        IT::Add(idTable, cur_iden);
                    }
                }
                if (cur_lex.lexema[0] == LEX_LITERAL) {
                    cur_iden.idtype = IT::L;
                    std::sprintf(cur_iden.id, "");

                    if (trueFlag) {
                        cur_iden.iddatatype = IT::LOGIC;
                        cur_iden.value.vint = 1;
                    } else if (falseFlag) {
                        cur_iden.iddatatype = IT::LOGIC;
                        cur_iden.value.vint = 0;
                    } else {
                        try {
                            if (strlen(str) > 2 && str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
                                cur_iden.iddatatype = IT::UNSIGNED;
                                cur_iden.value.vint = (unsigned int)stoi(str, nullptr, 16);
                            } else if (strlen(str) > 1 && str[0] == '0') {
                                cur_iden.iddatatype = IT::UNSIGNED;
                                cur_iden.value.vint = (unsigned int)stoi(str, nullptr, 8);
                            } else {
                                cur_iden.iddatatype = IT::UNSIGNED;
                                cur_iden.value.vint = (unsigned int)stoi(str);
                            }
                        } catch (const std::invalid_argument& ia) {
                             throw ERROR_THROW_IN(111, currentLine, pos); // Invalid symbol in literal
                        }
                    }

                    indexIT = IT::search(idTable, cur_iden);
                    if (indexIT >= 0) {
                        cur_lex.idxTI = indexIT;
                    } else {
                        std::sprintf(cur_iden.id, "L%d", number_literal++);
                        cur_iden.idxfirstLE = currentLine;
                        cur_iden.scope_name = cur_scope;
                        cur_lex.idxTI = idTable.size;
                        IT::Add(idTable, cur_iden);
                    }
                    trueFlag = false;
                    falseFlag = false;
                }
            }
            if (cur_lex.lexema[0] == LEX_ID) {
                cur_iden.idtype = IT::V;
                cur_iden.scope_name = cur_scope;
                cur_lex.idxTI = idTable.size;
                memcpy(cur_iden.id, str, 15);
                cur_iden.id[15] = '\0';
                cur_iden.iddatatype = IT::UNSIGNED;
                cur_iden.value.vint = NULL;
                cur_iden.idxfirstLE = currentLine;
                if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_FUNC) {
                    if (callFunc) {
                        throw ERROR_THROW_IN(600, currentLine, 0);
                    }
                    callFunc = true;
                    cur_iden.idtype = IT::F;
                    declareFunc = true;
                    if (intFlag) {
                        cur_iden.iddatatype = IT::UNSIGNED;
                        intFlag = false;
                        unsignedFlag = false;
                    }
                    if (logicFlag) {
                        cur_iden.iddatatype = IT::LOGIC;
                        logicFlag = false;
                    }
                    if (charFlag) {
                        throw ERROR_THROW_IN(126, currentLine, 0);
                        cur_iden.iddatatype = IT::CHAR;
                        charFlag = false;
                    }
                    indexIT = IT::search(idTable, cur_iden);
                    if (indexIT >= 0) {
                        throw ERROR_THROW_IN(107, currentLine, pos);
                    }
                    if (indexIT == -1) {
                        lexResult.functions[cur_iden.id] = {};
                        cur_lex.idxTI = idTable.size;
                        IT::Add(idTable, cur_iden);
                        pushToIT = true;
                    }
                }
                if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_LEFTTHESIS && lexTable.table[
                        lexTable.size - 3].lexema[0] == LEX_ID
                    && lexTable.table[lexTable.size - 3].idxTI == idTable.size - 1 && idTable.table[
                        idTable.size - 1].idtype == IT::F) {
                    cur_iden.idtype = IT::P;
                    countParms++;
                    if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_CHAR && charFlag) {
                        cur_iden.iddatatype = IT::CHAR;
                        std::strcpy(cur_iden.value.vstr.str, "");
                        charFlag = false;
                    }
                    if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_INTEGER && intFlag) {
                        cur_iden.iddatatype = IT::UNSIGNED;
                        std::strcpy(cur_iden.value.vstr.str, "");
                        intFlag = false;
                        unsignedFlag = false;
                    }
                    if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_LOGIC && logicFlag) {
                        cur_iden.iddatatype = IT::LOGIC;
                        std::strcpy(cur_iden.value.vstr.str, "");
                        logicFlag = false;
                    }
                    indexIT = IT::search(idTable, cur_iden);
                    if (indexIT >= 0) {
                        throw ERROR_THROW_IN(105, currentLine, pos);
                    }
                    lexResult.functions[cur_scope].push_back(cur_iden.iddatatype);
                    cur_lex.idxTI = idTable.size;
                    IT::Add(idTable, cur_iden);
                    pushToIT = true;
                    intFlag = false;
                    unsignedFlag = false;
                    logicFlag = false;
                    charFlag = false;
                }
                if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_COMMA && idTable.table[lexTable.table[
                        lexTable.size - 3].idxTI].idtype == IT::P) {
                    cur_iden.idtype = IT::P;
                    countParms++;
                    if (countParms > 8) {
                        throw ERROR_THROW_IN(108, currentLine, pos);
                    }
                    if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_CHAR && charFlag) {
                        cur_iden.iddatatype = IT::CHAR;
                        std::strcpy(cur_iden.value.vstr.str, "");
                        charFlag = false;
                    }
                    if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_INTEGER && intFlag && unsignedFlag) {
                        cur_iden.iddatatype = IT::UNSIGNED;
                        std::strcpy(cur_iden.value.vstr.str, "");
                        intFlag = false;
                        unsignedFlag = false;
                    }
                    if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_LOGIC && logicFlag) {
                        cur_iden.iddatatype = IT::LOGIC;
                        std::strcpy(cur_iden.value.vstr.str, "");
                        logicFlag = false;
                    }
                    indexIT = IT::search(idTable, cur_iden);
                    if (indexIT >= 0) {
                        throw ERROR_THROW_IN(105, currentLine, pos);
                    }
                    IT::Add(idTable, cur_iden);
                    lexResult.functions[cur_scope].push_back(cur_iden.iddatatype);
                    pushToIT = true;
                    unsignedFlag = false;
                    intFlag = false;
                    logicFlag = false;
                    charFlag = false;
                }
                if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_UNSIGNED && intFlag && unsignedFlag) {
                    cur_iden.iddatatype = IT::UNSIGNED;
                    std::strcpy(cur_iden.value.vstr.str, "");
                    indexIT = IT::search(idTable, cur_iden);
                    if (indexIT >= 0) {
                        throw ERROR_THROW_IN(105, currentLine, pos);
                    }
                    IT::Add(idTable, cur_iden);
                    pushToIT = true;
                    intFlag = false;
                    unsignedFlag = false;
                }
                if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_LOGIC && logicFlag) {
                    cur_iden.iddatatype = IT::LOGIC;
                    std::strcpy(cur_iden.value.vstr.str, "");
                    indexIT = IT::search(idTable, cur_iden);
                    if (indexIT >= 0) {
                        throw ERROR_THROW_IN(105, currentLine, pos);
                    }
                    IT::Add(idTable, cur_iden);
                    pushToIT = true;
                    logicFlag = false;
                }
                if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_CHAR && charFlag) {
                    cur_iden.iddatatype = IT::CHAR;
                    std::strcpy(cur_iden.value.vstr.str, "");
                    indexIT = IT::search(idTable, cur_iden);
                    if (indexIT >= 0) {
                        throw ERROR_THROW_IN(105, currentLine, pos);
                    }
                    IT::Add(idTable, cur_iden);
                    pushToIT = true;
                    charFlag = false;
                }
                if (!pushToIT) {
                    indexIT = IT::search(idTable, cur_iden, scope);
                    if (indexIT < 0) {
                        throw ERROR_THROW_IN(106, currentLine, pos);
                    }
                    cur_lex.idxTI = indexIT;
                }
                std::memset(cur_iden.id, NULL, 15);
                cur_iden.iddatatype = IT::UNSIGNED;
                cur_iden.value.vint = NULL;
                pushToIT = false;
            }
            if (cur_lex.lexema[0] == LEX_BECAUSE) {
                if (lexTable.table[lexTable.size - 1].lexema[0] == TYPE) {
                    throw ERROR_THROW_IN(104, currentLine, pos);
                }
                prev_scope = cur_scope;
                cur_scope = BECAUSE + to_string(number_until++);
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
            if (cur_lex.lexema[0] == LEX_DIFFER) {
                if (lexTable.table[lexTable.size - 1].lexema[0] == TYPE) {
                    throw ERROR_THROW_IN(104, currentLine, pos);
                }
                prev_scope = cur_scope;
                cur_scope = DIFFER + to_string(number_else++);
                scope[cur_scope] = prev_scope;
            }
            if (lexTable.table[lexTable.size - 1].lexema[0] == TYPE && cur_lex.lexema[0] != LEX_ID) {
                throw ERROR_THROW_IN(104, currentLine, pos);
            }
            if (in.text[i] != MARK) {
                bufferIndex = 0;
                std::memset(str, NULL, bufferIndex + 1);
            }
            if (cur_lex.lexema[0] != NULL) {
                cur_lex.sn = currentLine;
                LT::Add(lexTable, cur_lex);
                cur_lex.lexema[0] = NULL;
            }
            switch (in.text[i]) {
                case MARK:
                {
                    char literal_content_bytes[TI_STR_MAXSIZE] = {0}; // Buffer for UTF-8 content
                    int literal_content_len = 0;
                    int current_pos_in_line = pos; // Save current position for error reporting

                    // Advance past opening quote in the main loop iteration
                    i++;
                    pos++;

                    // Check for escape sequence
                    if (i < in.size && in.text[i] == '\\') {
                        i++; // Advance past '\'
                        pos++;
                        if (i >= in.size) { throw ERROR_THROW_IN(125, currentLine, current_pos_in_line); } // Error: Malformed escape
                        switch (in.text[i]) {
                            case 'n': literal_content_bytes[0] = '\n'; literal_content_len = 1; break;
                            case 't': literal_content_bytes[0] = '\t'; literal_content_len = 1; break;
                            case '\'': literal_content_bytes[0] = '\''; literal_content_len = 1; break;
                            case '\\': literal_content_bytes[0] = '\\'; literal_content_len = 1; break;
                            default: throw ERROR_THROW_IN(125, currentLine, current_pos_in_line); // Error: Unknown escape sequence
                        }
                    } else {
                        // Read potentially multi-byte character
                        int k = 0;
                        // Limit to 4 bytes for a single UTF-8 char, and check against `MARK`
                        while (k < 4 && (i + k) < in.size && in.text[i + k] != MARK) {
                            literal_content_bytes[k] = in.text[i + k];
                            literal_content_len++;
                            // Simple check to see if it's a complete multi-byte sequence
                            if ((literal_content_bytes[0] & 0x80) == 0) break; // ASCII (1 byte)
                            if (literal_content_len == 2 && (literal_content_bytes[0] & 0xE0) == 0xC0) break; // 2-byte
                            if (literal_content_len == 3 && (literal_content_bytes[0] & 0xF0) == 0xE0) break; // 3-byte
                            if (literal_content_len == 4 && (literal_content_bytes[0] & 0xF8) == 0xF0) break; // 4-byte

                            if (k >= 3) { // After 4 bytes, if not a complete sequence, something is wrong
                                throw ERROR_THROW_IN(125, currentLine, current_pos_in_line);
                            }
                            k++;
                        }
                    }

                    // Check for empty literal or multi-character literal
                    if (literal_content_len == 0) { // '' empty literal
                        throw ERROR_THROW_IN(125, currentLine, current_pos_in_line);
                    }

                    // After reading the char content, `i` is at the beginning of the char.
                    // Advance `i` past the character content.
                    i += literal_content_len;
                    pos += literal_content_len;

                    // Check for closing quote
                    if (i >= in.size || in.text[i] != MARK) {
                        throw ERROR_THROW_IN(125, currentLine, current_pos_in_line); // Error: Unterminated or malformed char literal
                    }

                    // `i` is now at the closing `MARK`. The main loop will increment `i` again, so no need here.

                    // Add to tables
                    cur_lex.lexema[0] = LEX_LITERAL;
                    cur_lex.sn = currentLine;

                    // Search for existing literal
                    bool found = false;
                    for (int k = 0; k < idTable.size; k++) {
                        if (idTable.table[k].idtype == IT::L && idTable.table[k].iddatatype == IT::CHAR &&
                            idTable.table[k].value.vstr.len == literal_content_len &&
                            strncmp(idTable.table[k].value.vstr.str, literal_content_bytes, literal_content_len) == 0) {
                            cur_lex.idxTI = k;
                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        cur_iden.idtype = IT::L;
                        cur_iden.iddatatype = IT::CHAR;
                        cur_iden.idxfirstLE = currentLine;

                        // Store in vstr.str
                        memcpy(cur_iden.value.vstr.str, literal_content_bytes, literal_content_len);
                        cur_iden.value.vstr.str[literal_content_len] = '\0';
                        cur_iden.value.vstr.len = literal_content_len;

                        std::sprintf(cur_iden.id, "L%d", number_literal++); // Assign unique ID
                        cur_iden.scope_name = cur_scope;
                        cur_lex.idxTI = idTable.size;
                        IT::Add(idTable, cur_iden);
                    }

                    // Ensure all flags are reset for next token processing
                    trueFlag = false;
                    falseFlag = false;

                    // cur_lex.lexema[0] is already LEX_LITERAL
                    // LT::Add will be called after the switch block.
                    // No need to set cur_lex.lexema[0] = NULL here.
                    break;
                }
                case NEW_LINE:
                    currentLine++;
                    cur_lex.lexema[0] = NULL;
                    pos = 0;
                    break;
                case LEX_SEMICOLON:
                    cur_lex.lexema[0] = LEX_SEMICOLON;
                    if (in.text[i - 1] == NEW_LINE) {
                        throw ERROR_THROW_IN(602, currentLine, 0);
                    }
                    cur_lex.sn = currentLine;
                    LT::Add(lexTable, cur_lex);
                    cur_lex.lexema[0] = NULL;
                    if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_BRACELET) {
                        if (cur_scope.find(BECAUSE) != -1) {
                            becauseFlag = false;
                        } else if (cur_scope.find(IF) != -1) {
                            ifFlag = false;
                        } else if (cur_scope.find(DIFFER) != -1) {
                            differFlag = false;
                        } else {
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
                    cur_lex.lexema[0] = NULL;
                    break;
                case LEX_LEFTBRACE:
                    if (mainFlag) {
                        scope["main"] = cur_scope;
                        prev_scope = cur_scope;
                        cur_scope = "main";
                        mainFlag = false;
                    }
                    cur_lex.lexema[0] = LEX_LEFTBRACE;
                    cur_lex.sn = currentLine;
                    LT::Add(lexTable, cur_lex);
                    cur_lex.lexema[0] = NULL;
                    break;
                case LEX_BRACELET:
                    cur_lex.lexema[0] = LEX_BRACELET;
                    if (in.text[i + 1] != ';') {
                        throw ERROR_THROW_IN(601, currentLine, 0);
                    }
                    cur_lex.sn = currentLine;
                    LT::Add(lexTable, cur_lex);
                    cur_lex.lexema[0] = NULL;
                    break;
                case LEX_LEFTTHESIS:
                    cur_lex.lexema[0] = LEX_LEFTTHESIS;
                    cur_lex.sn = currentLine;
                    LT::Add(lexTable, cur_lex);
                    cur_lex.lexema[0] = NULL;
                    if (becauseFlag || ifFlag) {
                        continue;
                    } else if (declareFunc) {
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
                    cur_lex.lexema[0] = NULL;
                    parmFlag = false;
                    break;
                case LEX_PLUS:
                    if (in.text[i + 1] == LEX_PLUS) {
                        cur_lex.lexema[0] = LEX_INC;
                        i++;
                    } else {
                        cur_lex.lexema[0] = LEX_PLUS;
                    }
                    cur_lex.sn = currentLine;
                    LT::Add(lexTable, cur_lex);
                    cur_lex.lexema[0] = NULL;
                    break;
                case LEX_MINUS:
                    if (in.text[i + 1] == LEX_MINUS) {
                        cur_lex.lexema[0] = LEX_DEC;
                        i++;
                    } else {
                        cur_lex.lexema[0] = LEX_MINUS;
                    }
                    cur_lex.sn = currentLine;
                    LT::Add(lexTable, cur_lex);
                    cur_lex.lexema[0] = NULL;
                    break;
                case LEX_STAR:
                    cur_lex.lexema[0] = LEX_STAR;
                    cur_lex.sn = currentLine;
                    LT::Add(lexTable, cur_lex);
                    cur_lex.lexema[0] = NULL;
                    break;
                case LEX_COLON:
                    cur_lex.lexema[0] = LEX_COLON;
                    cur_lex.sn = currentLine;
                    LT::Add(lexTable, cur_lex);
                    cur_lex.lexema[0] = NULL;
                    break;
                case LEX_BIT_NOT:
                    cur_lex.lexema[0] = LEX_BIT_NOT;
                    cur_lex.sn = currentLine;
                    LT::Add(lexTable, cur_lex);
                    cur_lex.lexema[0] = NULL;
                    break;
                case LEX_EQUAL:
                    if (in.text[i + 1] == LEX_EQUAL && in.text[i - 1] != LEX_EQUAL) {
                        cur_lex.lexema[0] = LEX_ISEQUAL;
                        i++;
                        cur_lex.sn = currentLine;
                        LT::Add(lexTable, cur_lex);
                        cur_lex.lexema[0] = NULL;
                        break;
                    }
                    if (in.text[i - 1] != LEX_EQUAL && in.text[i + 1] != LEX_EQUAL) {
                        cur_lex.lexema[0] = LEX_EQUAL;
                        if (lexTable.table[lexTable.size - 1].lexema[0] != LEX_ID) {
                            throw ERROR_THROW_IN(602, currentLine, pos);
                        }
                        cur_lex.sn = currentLine;
                        LT::Add(lexTable, cur_lex);
                        cur_lex.lexema[0] = NULL;
                        break;
                    } else {
                        throw ERROR_THROW_IN(601, currentLine, pos);
                    }
                case LEX_NOT:
                    if (in.text[i + 1] == LEX_EQUAL) {
                        cur_lex.lexema[0] = LEX_NOT_EQUAL;
                        i++;
                    } else {
                        throw ERROR_THROW_IN(601, currentLine, pos);
                    }
                    cur_lex.sn = currentLine;
                    LT::Add(lexTable, cur_lex);
                    cur_lex.lexema[0] = NULL;
                    break;
                case LEX_MORE:
                    cur_lex.lexema[0] = LEX_MORE;
                    if (in.text[i + 1] == EQUAL) {
                        cur_lex.lexema[0] = LEX_MORE_OR_EQUAL;
                        i++;
                    }
                    cur_lex.sn = currentLine;
                    LT::Add(lexTable, cur_lex);
                    cur_lex.lexema[0] = NULL;
                    break;
                case LEX_LESS:
                    cur_lex.lexema[0] = LEX_LESS;
                    if (in.text[i + 1] == EQUAL) {
                        cur_lex.lexema[0] = LEX_LESS_OR_EQUAL;
                        i++;
                    }
                    cur_lex.sn = currentLine;
                    LT::Add(lexTable, cur_lex);
                    cur_lex.lexema[0] = NULL;
                    break;
                case LEX_OST:
                    cur_lex.lexema[0] = LEX_OST;
                    cur_lex.sn = currentLine;
                    LT::Add(lexTable, cur_lex);
                    cur_lex.lexema[0] = NULL;
                    break;
            }
        }

        currentLine = 1;
        LT_file << currentLine;
        LT_file << '\t';
        for (int i = 0; i < lexTable.size; i++) {
            cur_lex = LT::GetEntry(lexTable, i);
            if (currentLine != cur_lex.sn) {
                currentLine = cur_lex.sn;
                LT_file << '\n';
                LT_file << currentLine;
                LT_file << '\t';
            }
            LT_file << cur_lex.lexema[0];
        }
        LT_file
                .
                close();

        IT_file
                <<
                std::setw(
                    10
                )
                <<
                "id"
                <<
                std::setw(
                    20
                )
                <<
                "datatype"
                <<
                std::setw(
                    20
                )
                <<
                "idtype"
                <<
                std::setw(
                    20
                )
                <<
                "Line"
                <<
                std::setw(
                    20
                )
                <<
                "value"
                <<
                std::setw(
                    20
                )
                <<
                "Scope"
                <<
                std::setw(
                    20
                )
                <<
                "idTi"
                <<
                std::endl;

        for
        (
            int i = 0;
            i < idTable.size;
            i
            ++
        ) {
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

            if ((cur_iden.iddatatype == IT::UNSIGNED || cur_iden.iddatatype ==
                 IT::LOGIC) && cur_iden.idtype == IT::L) {
                IT_file << std::setw(20) << cur_iden.value.vint;
            }
            if ((cur_iden.iddatatype == IT::UNSIGNED || cur_iden.iddatatype == IT::LOGIC
                 || cur_iden.iddatatype == IT::CHAR) && cur_iden.idtype != IT::L) {
                IT_file << std::setw(20) << ' ';
            }
            if (cur_iden.iddatatype == IT::CHAR && cur_iden.idtype == IT::L) {
                IT_file << std::setw(16);
                for (int j = 0; j < strlen(cur_iden.value.vstr.str); j++) {
                    IT_file << cur_iden.value.vstr.str[j];
                }
                IT_file << std::setw(20);
            }
            IT_file << std::setw(20) << cur_iden.scope_name;
            IT_file << std::setw(20);
            IT_file << cur_iden.idxfirstLE;
            IT_file << std::endl;
        }
        IT_file
                .
                close();

        lexResult
                .
                idTable = idTable;
        lexResult
                .
                lexTable = lexTable;
        return
                lexResult;
    }
}
