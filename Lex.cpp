#include "Lex.h"
#include "Parm.h"

#include <cstring>

#include "stdafx.h"
#include "In.h"
#include "Error.h"
#include <stdio.h>
#include <string.h>
#include <iomanip>
#include <vector>
#include <map>

// лексический анализ
namespace Lex {
    LT::LexTable lexTable = LT::Create(LT_MAXSIZE - 1);
    IT::IdTable idTable = IT::Create(TI_MAXSIZE - 1);

    int countParms = 0;
    bool differFlag = false;
    bool ifFlag = false;
    bool elseflag = false;
    bool charFlag = false;
    bool unsignedIntFlag = false;
    bool logicFlag = false;
    bool parmFlag = false;
    bool litFlag = false;
    bool falseFlag = false;
    bool trueFlag = false;
    bool mainFlag = false;
    bool becauseFlag = false;
    bool callFunc = false;

    char *str = new char[TI_MAXSIZE];
    bool declareFlag = false;
    bool declareFunc = false; // объявление заголовка
    bool pushToIT = false;

    char FST() {
        FST_UNSIGNED_INTEGER FST_CHAR FST_LOGIC FST_FUNC FST_SEND FST_MAIN FST_WRITECH FST_READCH FST_TRUE FST_FALSE FST_BECAUSE FST_IF FST_LITERAL FST_IDEN FST_DIFFER
        if (FST::execute(_unsigned_integer)) { unsignedIntFlag = true; return LEX_UNSIGNED_INTEGER; }
        if (FST::execute(_logic)) { logicFlag = true; return LEX_LOGIC; }
        if (FST::execute(_char)) { charFlag = true; return LEX_CHAR; }
        if (FST::execute(_func)) return LEX_FUNC;
        if (FST::execute(_send)) return LEX_SEND;
        if (FST::execute(_main)) { mainFlag = true; return LEX_MAIN; }
        if (FST::execute(_writech)) return LEX_WRITECH;
        if (FST::execute(_readch)) return LEX_READCH;
        if (FST::execute(_true)) { trueFlag = true; return LEX_LITERAL; }
        if (FST::execute(_false)) { falseFlag = true; return LEX_LITERAL; }
        if (FST::execute(_because)) { becauseFlag = true; return LEX_BECAUSE; }
        if (FST::execute(_if)) { ifFlag = true; return LEX_IF; }
        if (FST::execute(_differ)) { differFlag = true; return LEX_DIFFER; }
        if (FST::execute(_literal)) return LEX_LITERAL;
        if (FST::execute(_iden)) return LEX_ID;
        return '\0';
    }

    LEX Lex(Parm::Parm parm, In::IN in) {
        LEX lexResult;

        // очистка таблиц
        lexTable.size = 0;
        idTable.size = 0;

        // регистрация функций стандартной библиотеки
        auto addStFunc = [&](const char* name, IT::IDDATATYPE retType, std::vector<IT::IDDATATYPE> parms) {
            IT::Entry entry(-1, name, retType, IT::F, "global", 0);
            IT::Add(idTable, entry);
            lexResult.functions[name] = parms;
        };

        addStFunc("sqrt", IT::UNSIGNED, { IT::UNSIGNED });
        addStFunc("getMin", IT::UNSIGNED, { IT::UNSIGNED, IT::UNSIGNED });
        addStFunc("getMax", IT::UNSIGNED, { IT::UNSIGNED, IT::UNSIGNED });
        addStFunc("isPrime", IT::LOGIC, { IT::UNSIGNED });
        addStFunc("toUpper", IT::CHAR, { IT::CHAR });
        addStFunc("pow", IT::UNSIGNED, { IT::UNSIGNED, IT::UNSIGNED });

        int indexIT;
        int pos = 0;
        LT::Entry cur_lex;
        int bufferIndex = 0;
        cur_lex.sn = 0;
        cur_lex.idxTI = 0;
        cur_lex.lexema[0] = '\0';
        int number_literal = 0;
        int count_because = 0;
        int count_if = 0;
        int count_differ = 0;

        map<string, string> scope;
        scope["global"] = "";
        string cur_scope = "global";
        string prev_scope = "";

        IT::Entry cur_iden;

        int currentLine = 1;
        ofstream LT_file("LT.txt");
        ofstream IT_file("IT.txt");

        std::memset(str, 0, TI_MAXSIZE);

        for (int i = 0; i < in.size; i++) {
            pos++;

            bool isValidChar = (in.text[i] >= 'A' && in.text[i] <= 'Z') ||
                               (in.text[i] >= 'a' && in.text[i] <= 'z') ||
                               (in.text[i] >= '0' && in.text[i] <= '9') ||
                               (in.text[i] >= 0xC0 && in.text[i] <= 0xFF) ||
                               (in.text[i] >= 0xE0 && in.text[i] <= 0xFF) ||
                               in.text[i] == DOT;

            bool isUnsignedSpace = (in.text[i] == SPACE && bufferIndex == 8 && strncmp(str, "unsigned", 8) == 0);
            bool isLiteralBody = (in.text[i] != MARK && litFlag);
            bool isLiteralStart = (in.text[i] == MARK && !litFlag);

            if ((isValidChar && in.text[i] != SPACE) || isUnsignedSpace || isLiteralStart || isLiteralBody) {
                if (in.text[i] == MARK) litFlag = true;
                str[bufferIndex] = in.text[i];
                bufferIndex++;
                if (bufferIndex >= TI_MAXSIZE) throw ERROR_THROW(119);
            } else {
                str[bufferIndex] = '\0';

                if (bufferIndex > 0) cur_lex.lexema[0] = FST();
                else cur_lex.lexema[0] = '\0';

                // MAIN
                if (cur_lex.lexema[0] == LEX_MAIN) {
                    if (callFunc) throw ERROR_THROW_IN(600, currentLine, 0);
                    mainFlag = true;
                    cur_lex.idxTI = idTable.size;

                    cur_iden = IT::Entry();
                    std::strncpy(cur_iden.id, str, 15);
                    cur_iden.iddatatype = IT::UNSIGNED;
                    cur_iden.idtype = IT::F;
                    cur_iden.value.vint = 0;
                    cur_iden.idxfirstLE = currentLine;
                    cur_iden.scope_name = cur_scope;

                    if (IT::search(idTable, cur_iden) >= 0) throw ERROR_THROW_IN(309, currentLine, pos);
                    IT::Add(idTable, cur_iden);
                }

                // LITERAL
                if (cur_lex.lexema[0] == LEX_LITERAL) {
                    cur_iden = IT::Entry();
                    cur_iden.iddatatype = IT::UNSIGNED;
                    cur_iden.idtype = IT::L;
                    cur_iden.idxfirstLE = currentLine;
                    cur_iden.scope_name = cur_scope;

                    bool dot = false;
                    for (int j = 0; j < strlen(str); j++) if (str[j] == '.') dot = true;

                    if (trueFlag) {
                        cur_iden.iddatatype = IT::LOGIC; cur_iden.value.vint = 1; trueFlag = false;
                    } else if (falseFlag) {
                        cur_iden.iddatatype = IT::LOGIC; cur_iden.value.vint = 0; falseFlag = false;
                    } else {
                        bool isNegative = false;
                        if (lexTable.size >= 2) {
                            int len = (int)strlen(str);
                            if (i - len - 1 >= 0 &&
                                lexTable.table[lexTable.size - 2].lexema[0] != LEX_RIGHTTHESIS &&
                                lexTable.table[lexTable.size - 2].lexema[0] != LEX_ID &&
                                lexTable.table[lexTable.size - 2].lexema[0] != LEX_LITERAL &&
                                in.text[i - len] != ' ' && in.text[i - len - 1] == LEX_MINUS) {
                                isNegative = true;
                            }
                        }

                        try {
                            if (isNegative) {
                                // Ошибка: присвоение отрицательного числа беззнаковому типу
                                throw ERROR_THROW_IN(126, currentLine, pos);
                            } else {
                                size_t idx = 0;
                                if (str[0] == '0' && str[1] == 'x') {
                                    cur_iden.value.vint = (unsigned int)std::stoul(str, &idx, 16);
                                } else {
                                    cur_iden.value.vint = (unsigned int)std::stoul(str, &idx, 10);
                                }
                                
                                if (idx != strlen(str)) {
                                    // Ошибка: некорректный литерал (например 1A или 0x)
                                    throw ERROR_THROW_IN(111, currentLine, pos);
                                }
                            }
                        } catch (...) {
                            throw ERROR_THROW_IN(126, currentLine, pos);
                        }
                    }

                    indexIT = IT::search(idTable, cur_iden);
                    if (indexIT >= 0) {
                        cur_lex.idxTI = indexIT;
                    } else {
                        std::sprintf(cur_iden.id, "L%d", number_literal++);
                        cur_lex.idxTI = idTable.size;
                        IT::Add(idTable, cur_iden);
                    }
                }

                // IDENTIFIERS
                if (cur_lex.lexema[0] == LEX_ID) {
                    cur_iden = IT::Entry();
                    std::strncpy(cur_iden.id, str, 15);
                    cur_iden.idtype = IT::V;
                    cur_iden.scope_name = cur_scope;
                    cur_iden.iddatatype = IT::UNSIGNED;
                    cur_iden.idxfirstLE = currentLine;

                    // 1. Объявление функции
                    if (lexTable.size >= 2 && lexTable.table[lexTable.size - 2].lexema[0] == LEX_FUNC) {
                        if (callFunc) throw ERROR_THROW_IN(600, currentLine, 0);
                        callFunc = true;
                        cur_iden.idtype = IT::F;
                        declareFunc = true; // Начало объявления функции

                        if (unsignedIntFlag) { cur_iden.iddatatype = IT::UNSIGNED; unsignedIntFlag = false; }
                        else if (logicFlag) { cur_iden.iddatatype = IT::LOGIC; logicFlag = false; }
                        else if (charFlag) { cur_iden.iddatatype = IT::CHAR; charFlag = false; }

                        if (IT::search(idTable, cur_iden) >= 0) throw ERROR_THROW_IN(307, currentLine, pos);
                        lexResult.functions[cur_iden.id] = {};

                        cur_lex.idxTI = idTable.size;
                        IT::Add(idTable, cur_iden);
                        pushToIT = true;
                    }

                    // 2. Объявление параметров (первый)
                    // ВАЖНО: Добавлена проверка declareFunc, чтобы не путать с вызовом функции
                    if (declareFunc && lexTable.size >= 3 &&
                        lexTable.table[lexTable.size - 2].lexema[0] == LEX_LEFTTHESIS &&
                        lexTable.table[lexTable.size - 3].lexema[0] == LEX_ID &&
                        lexTable.table[lexTable.size - 3].idxTI == idTable.size - 1 &&
                        idTable.table[idTable.size - 1].idtype == IT::F) {

                        cur_iden.idtype = IT::P;
                        countParms++;
                        if (charFlag) { cur_iden.iddatatype = IT::CHAR; charFlag = false; }
                        else if (unsignedIntFlag) { cur_iden.iddatatype = IT::UNSIGNED; unsignedIntFlag = false; }
                        else if (logicFlag) { cur_iden.iddatatype = IT::LOGIC; logicFlag = false; }

                        if (IT::search(idTable, cur_iden) >= 0) throw ERROR_THROW_IN(305, currentLine, pos);
                        lexResult.functions[cur_scope].push_back(cur_iden.iddatatype);

                        cur_lex.idxTI = idTable.size;
                        IT::Add(idTable, cur_iden);
                        pushToIT = true;
                    }

                    // 3. Объявление параметров (следующие)
                    if (declareFunc && lexTable.size >= 3 &&
                        lexTable.table[lexTable.size - 2].lexema[0] == LEX_COMMA &&
                        idTable.table[lexTable.table[lexTable.size - 3].idxTI].idtype == IT::P) {

                        cur_iden.idtype = IT::P;
                        countParms++;
                        if (countParms > 8) throw ERROR_THROW_IN(308, currentLine, pos);

                        if (charFlag) { cur_iden.iddatatype = IT::CHAR; charFlag = false; }
                        else if (unsignedIntFlag) { cur_iden.iddatatype = IT::UNSIGNED; unsignedIntFlag = false; }
                        else if (logicFlag) { cur_iden.iddatatype = IT::LOGIC; logicFlag = false; }

                        if (IT::search(idTable, cur_iden) >= 0) throw ERROR_THROW_IN(305, currentLine, pos);

                        cur_lex.idxTI = idTable.size;
                        IT::Add(idTable, cur_iden);
                        lexResult.functions[cur_scope].push_back(cur_iden.iddatatype);
                        pushToIT = true;
                    }

                    // 4. Объявление переменных
                    if (unsignedIntFlag) {
                        cur_iden.iddatatype = IT::UNSIGNED; unsignedIntFlag = false;
                        if (IT::search(idTable, cur_iden) >= 0) throw ERROR_THROW_IN(305, currentLine, pos);
                        cur_lex.idxTI = idTable.size;
                        IT::Add(idTable, cur_iden);
                        pushToIT = true;
                    }
                    else if (logicFlag) {
                        cur_iden.iddatatype = IT::LOGIC; logicFlag = false;
                        if (IT::search(idTable, cur_iden) >= 0) throw ERROR_THROW_IN(305, currentLine, pos);
                        cur_lex.idxTI = idTable.size;
                        IT::Add(idTable, cur_iden);
                        pushToIT = true;
                    }
                    else if (charFlag) {
                        cur_iden.iddatatype = IT::CHAR; charFlag = false;
                        if (IT::search(idTable, cur_iden) >= 0) throw ERROR_THROW_IN(305, currentLine, pos);
                        cur_lex.idxTI = idTable.size;
                        IT::Add(idTable, cur_iden);
                        pushToIT = true;
                    }

                    // 5. Использование
                    if (!pushToIT) {
                        IT::Entry searchEntry;
                        std::strncpy(searchEntry.id, str, 15);
                        searchEntry.id[15] = '\0';
                        searchEntry.scope_name = cur_scope;

                        indexIT = IT::search(idTable, searchEntry, scope);
                        if (indexIT < 0) throw ERROR_THROW_IN(306, currentLine, pos);
                        cur_lex.idxTI = indexIT;
                    }

                    pushToIT = false;
                }

                // Blocks
                if (cur_lex.lexema[0] == LEX_BECAUSE) {
                    if (lexTable.size > 0 && lexTable.table[lexTable.size - 1].lexema[0] == TYPE) throw ERROR_THROW_IN(304, currentLine, pos);
                    prev_scope = cur_scope;
                    cur_scope = BECAUSE + to_string(count_because++);
                    scope[cur_scope] = prev_scope;
                }
                if (cur_lex.lexema[0] == LEX_IF) {
                    if (lexTable.size > 0 && lexTable.table[lexTable.size - 1].lexema[0] == TYPE) throw ERROR_THROW_IN(304, currentLine, pos);
                    prev_scope = cur_scope;
                    cur_scope = IF + to_string(count_if++);
                    scope[cur_scope] = prev_scope;
                }
                if (cur_lex.lexema[0] == LEX_DIFFER) {
                    if (lexTable.size > 0 && lexTable.table[lexTable.size - 1].lexema[0] == TYPE) throw ERROR_THROW_IN(304, currentLine, pos);
                    prev_scope = cur_scope;
                    cur_scope = DIFFER + to_string(count_differ++);
                    scope[cur_scope] = prev_scope;
                }

                if (in.text[i] != MARK) {
                    bufferIndex = 0;
                    std::memset(str, 0, bufferIndex + 1);
                }
            }

            // Добавление в таблицу
            if (cur_lex.lexema[0] != '\0') {
                cur_lex.sn = currentLine;
                LT::Add(lexTable, cur_lex);
                cur_lex.lexema[0] = '\0';
            }

            if (litFlag && in.text[i] == MARK && bufferIndex == 1) continue;
            else if (litFlag && in.text[i] != MARK) {
                if (in.text[i] == '`') throw ERROR_THROW_IN(125, currentLine, pos);
                continue;
            }

            switch (in.text[i]) {
                case MARK:
                    litFlag = true;
                    if (str[0] == MARK) {
                        cur_lex.idxTI = idTable.size;
                        str[bufferIndex] = MARK; bufferIndex++; str[bufferIndex] = '\0';
                        litFlag = false;

                        cur_lex.lexema[0] = LEX_LITERAL;
                        cur_iden = IT::Entry();
                        std::sprintf(cur_iden.id, "L%d", number_literal++);
                        cur_iden.iddatatype = IT::CHAR;
                        cur_iden.idtype = IT::L;
                        cur_iden.idxfirstLE = currentLine;
                        cur_iden.scope_name = cur_scope;

                        // обработка символьных литералов
                        if (str[1] == '\\' && str[2] == 'n') {
                            cur_iden.value.vstr.str[0] = '\n';
                            cur_iden.value.vstr.len = 1;
                        } else if (str[1] == '\\' && str[2] == 't') { // обработка \t
                            cur_iden.value.vstr.str[0] = '\t';
                            cur_iden.value.vstr.len = 1;
                        } else if (str[1] == '\\' && strlen(str) == 4) {
                            // недопустимая escape-последовательность
                            throw ERROR_THROW_IN(127, currentLine, pos);
                        }
                        else {
                            // если это обычный символ
                            if (strlen(str) == 3) { 
                                cur_iden.value.vstr.str[0] = str[1];
                                cur_iden.value.vstr.len = 1;
                            } else if (strlen(str) == 2) { 
                                // пустой символьный литерал, трактуем как пробел
                                cur_iden.value.vstr.str[0] = ' '; 
                                cur_iden.value.vstr.len = 1;
                            } else if (strlen(str) == 4) {
                                // UTF-8 char literal -> CP1251
                                unsigned char u1 = (unsigned char)str[1];
                                unsigned char u2 = (unsigned char)str[2];
                                if (u1 == 0xD0 && u2 == 0x81) cur_iden.value.vstr.str[0] = (char)0xA8; // Ё
                                else if (u1 == 0xD1 && u2 == 0x91) cur_iden.value.vstr.str[0] = (char)0xB8; // ё
                                else if (u1 == 0xD0 && u2 >= 0x90 && u2 <= 0xBF) cur_iden.value.vstr.str[0] = (char)(u2 + 0x30);
                                else if (u1 == 0xD1 && u2 >= 0x80 && u2 <= 0x8F) cur_iden.value.vstr.str[0] = (char)(u2 + 0x70);
                                else cur_iden.value.vstr.str[0] = '?'; // Unsupported
                                cur_iden.value.vstr.len = 1;
                            }
                            else {
                                // некорректный символьный литерал
                                throw ERROR_THROW_IN(125, currentLine, pos); 
                            }
                        }
                        // завершающий нуль для безопасности
                        cur_iden.value.vstr.str[1] = '\0';

                        cur_lex.sn = currentLine;
                        LT::Add(lexTable, cur_lex);
                        IT::Add(idTable, cur_iden);

                        bufferIndex = 0; std::memset(str, 0, bufferIndex + 1); cur_lex.lexema[0] = '\0';
                    }
                    break;
                case '\n':
                case NEW_LINE: currentLine++; cur_lex.lexema[0] = '\0'; pos = 0; break;
                case ' ': case '\t': case '\r': break;
                case LEX_SEMICOLON:
                    cur_lex.lexema[0] = LEX_SEMICOLON;
                    if (i > 0 && in.text[i - 1] == NEW_LINE) throw ERROR_THROW_IN(602, currentLine, 0);
                    cur_lex.sn = currentLine;
                    LT::Add(lexTable, cur_lex);
                    cur_lex.lexema[0] = '\0';

                    if (lexTable.size >= 2 && lexTable.table[lexTable.size - 2].lexema[0] == LEX_BRACELET) {
                        if (cur_scope.find(BECAUSE) != string::npos) differFlag = false;
                        else if (cur_scope.find(IF) != string::npos) ifFlag = false;
                        else if (cur_scope.find(DIFFER) != string::npos) elseflag = false;
                        else {
                            // declareFunc = false; // Убрано отсюда, перенесено в '{'
                            callFunc = false;
                        }

                        cur_scope = prev_scope;
                        prev_scope = scope[cur_scope];
                    }
                    break;
                case LEX_COMMA: cur_lex.lexema[0] = LEX_COMMA; cur_lex.sn = currentLine; LT::Add(lexTable, cur_lex); cur_lex.lexema[0] = '\0'; break;
                case LEX_LEFTBRACE:
                    if (mainFlag) { scope["main"] = cur_scope; prev_scope = cur_scope; cur_scope = "main"; mainFlag = false; }
                    // завершаем фазу объявления
                    if (declareFunc) { declareFunc = false; }
                    cur_lex.lexema[0] = LEX_LEFTBRACE; cur_lex.sn = currentLine; LT::Add(lexTable, cur_lex); cur_lex.lexema[0] = '\0'; break;
                case LEX_BRACELET:
                    cur_lex.lexema[0] = LEX_BRACELET;
                    if (in.text[i + 1] != ';') throw ERROR_THROW_IN(601, currentLine, 0);
                    cur_lex.sn = currentLine; LT::Add(lexTable, cur_lex); cur_lex.lexema[0] = '\0'; break;
                case LEX_LEFTTHESIS:
                    cur_lex.lexema[0] = LEX_LEFTTHESIS; cur_lex.sn = currentLine; LT::Add(lexTable, cur_lex); cur_lex.lexema[0] = '\0';
                    if (differFlag || ifFlag) continue;
                    else if (declareFunc) {
                        parmFlag = true;
                        for (int k = idTable.size - 1; k >= 0; k--) {
                            if (idTable.table[k].idtype == IT::F) {
                                scope[idTable.table[k].id] = cur_scope;
                                prev_scope = cur_scope;
                                cur_scope = idTable.table[k].id;
                                countParms = 0;
                                break;
                            }
                        }
                    }
                    break;
                case LEX_RIGHTTHESIS: cur_lex.lexema[0] = LEX_RIGHTTHESIS; cur_lex.sn = currentLine; LT::Add(lexTable, cur_lex); cur_lex.lexema[0] = '\0'; parmFlag = false; break;
                case LEX_PLUS:
                    if (in.text[i + 1] ==LEX_PLUS) { cur_lex.lexema[0] = LEX_INC; i++; }
                    else cur_lex.lexema[0] = LEX_PLUS;
                    cur_lex.sn = currentLine; LT::Add(lexTable, cur_lex); cur_lex.lexema[0] = '\0'; break;
                case LEX_MINUS:
                    if (in.text[i + 1] ==LEX_MINUS) { cur_lex.lexema[0] = LEX_DEC; i++; }
                    else cur_lex.lexema[0] = LEX_MINUS;
                    cur_lex.sn = currentLine; LT::Add(lexTable, cur_lex); cur_lex.lexema[0] = '\0'; break;
                case LEX_STAR: cur_lex.lexema[0] = LEX_STAR; cur_lex.sn = currentLine; LT::Add(lexTable, cur_lex); cur_lex.lexema[0] = '\0'; break;
                case LEX_COLON: cur_lex.lexema[0] = LEX_COLON; cur_lex.sn = currentLine; LT::Add(lexTable, cur_lex); cur_lex.lexema[0] = '\0'; break;
                case LEX_EQUAL:
                    if (in.text[i + 1] ==LEX_EQUAL && in.text[i - 1] !=LEX_EQUAL) { cur_lex.lexema[0] = LEX_ISEQUAL; i++; cur_lex.sn = currentLine; LT::Add(lexTable, cur_lex); cur_lex.lexema[0] = '\0'; break; }
                    if (in.text[i - 1] !=LEX_EQUAL && in.text[i + 1] !=LEX_EQUAL) {
                        cur_lex.lexema[0] = LEX_EQUAL;
                        if (lexTable.size > 0 && lexTable.table[lexTable.size - 1].lexema[0] !=LEX_ID) throw ERROR_THROW_IN(602, currentLine, pos);
                        cur_lex.sn = currentLine; LT::Add(lexTable, cur_lex); cur_lex.lexema[0] = '\0'; break;
                    } else throw ERROR_THROW_IN(601, currentLine, pos);
                case LEX_BIT_NOT: cur_lex.lexema[0] = LEX_BIT_NOT; cur_lex.sn = currentLine; LT::Add(lexTable, cur_lex); cur_lex.lexema[0] = '\0'; break;
                case LEX_NOT:
                    if (in.text[i + 1] == LEX_EQUAL) { cur_lex.lexema[0] = LEX_NOT_EQUAL; i++; } else throw ERROR_THROW_IN(601, currentLine, pos);
                    cur_lex.sn = currentLine; LT::Add(lexTable, cur_lex); cur_lex.lexema[0] = '\0'; break;
                case LEX_MORE:
                    cur_lex.lexema[0] = LEX_MORE; if (in.text[i + 1] == EQUAL) { cur_lex.lexema[0] = LEX_MORE_OR_EQUAL; i++; }
                    cur_lex.sn = currentLine; LT::Add(lexTable, cur_lex); cur_lex.lexema[0] = '\0'; break;
                case LEX_LESS:
                    cur_lex.lexema[0] = LEX_LESS; if (in.text[i + 1] == EQUAL) { cur_lex.lexema[0] = LEX_LESS_OR_EQUAL; i++; }
                    cur_lex.sn = currentLine; LT::Add(lexTable, cur_lex); cur_lex.lexema[0] = '\0'; break;
                case LEX_OST: cur_lex.lexema[0] = LEX_OST; cur_lex.sn = currentLine; LT::Add(lexTable, cur_lex); cur_lex.lexema[0] = '\0'; break;
            }
        }

        currentLine = 1;
        LT_file << currentLine << '\t';
        for (int i = 0; i < lexTable.size; i++) {
            cur_lex = LT::GetEntry(lexTable, i);
            if (currentLine != cur_lex.sn) { currentLine = cur_lex.sn; LT_file << '\n' << currentLine << '\t'; }
            LT_file << cur_lex.lexema[0];
        }
        LT_file.close();

        IT_file << std::setw(10) << "id" << std::setw(20) << "datatype" << std::setw(20) << "idtype" << std::setw(20) << "Line" << std::setw(20) << "value" << std::setw(20) << "Scope" << std::setw(20) << "idTi" << std::endl;
        for (int i = 0; i < idTable.size; i++) {
            cur_iden = IT::GetEntry(idTable, i);
            IT_file << std::setw(10) << cur_iden.id;
            if (cur_iden.iddatatype == IT::UNSIGNED) IT_file << std::setw(20) << "UNSIGNED";
            else if (cur_iden.iddatatype == IT::LOGIC) IT_file << std::setw(20) << "LOGIC";
            else if (cur_iden.iddatatype == IT::CHAR) IT_file << std::setw(20) << "CHAR";

            if (cur_iden.idtype == IT::V) IT_file << std::setw(20) << "V";
            else if (cur_iden.idtype == IT::L) IT_file << std::setw(20) << "L";
            else if (cur_iden.idtype == IT::F) IT_file << std::setw(20) << "F";
            else if (cur_iden.idtype == IT::P) IT_file << std::setw(20) << "P";
            IT_file << std::setw(20) << cur_iden.idxfirstLE;

            if ((cur_iden.iddatatype == IT::UNSIGNED || cur_iden.iddatatype == IT::LOGIC) && cur_iden.idtype == IT::L) IT_file << std::setw(20) << cur_iden.value.vint;
            else IT_file << std::setw(20) << ' ';

            if (cur_iden.iddatatype == IT::CHAR && cur_iden.idtype == IT::L) {
                IT_file << std::setw(16) << cur_iden.value.vstr.str << std::setw(20);
            }
            IT_file << std::setw(20) << cur_iden.scope_name << std::setw(20) << cur_iden.idxfirstLE << std::endl;
        }
        IT_file.close();

        lexResult.idTable = idTable;
        lexResult.lexTable = lexTable;
        return lexResult;
    }
}