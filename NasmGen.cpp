#include "NasmGen.h"
#include "Error.h"
#include <iomanip>
#include <ctime>
#include <stack>
#include <algorithm>
#include <vector>

using namespace std;

// генератор кода
namespace NasmGen {

// путь к статической библиотеке
const string LIB_PATH = "/home/euphoo/02. University/КП/LSA_2025/LSA_StdLib/build/libLSA_StdLib.a";

// управление блоками
struct LabelBlock {
	int type;
	int label_1;
	int label_2;
	int paramSize;
};

// заголовок asm
const string ASM_HEAD = 
	"global main\n"
	"default rel\n"
	"extern lsa_writech, lsa_writeuint, lsa_writelogic, lsa_readch, lsa_pow, lsa_sqrt, lsa_isPrime, lsa_getMin, lsa_getMax, lsa_toUpper\n"
	"section .text\n\n";

// stdlib теперь внешняя

// декорирование имен
string getMangledName(IT::Entry& entry) {
	return entry.scope_name + "_" + entry.id;
}

bool isStandardFunc(const string& name) {
	static const vector<string> stdFuncs = { "pow", "sqrt", "getMin", "getMax", "isPrime", "toUpper", "readch", "writech", "writeuint", "writelogic" };
	return find(stdFuncs.begin(), stdFuncs.end(), name) != stdFuncs.end();
}

int getStdFuncParamCount(const string& name) {
	if (name == "pow" || name == "getMin" || name == "getMax") return 2;
	if (name == "readch") return 0;
	return 1; // sqrt, isPrime, toUpper, writech, writeuint, writelogic
}

// генерация выражения
void processExpression(int start, int end, Lex::LEX& lex, ofstream& file) {
	int stackDepth = 0;
	for (int i = start; i < end; i++) {
		LT::Entry& t = lex.lexTable.table[i];
		if (t.lexema[0] == '\0') continue;

		switch (t.lexema[0]) {
		// литералы
		case LEX_LITERAL: {
			if (t.idxTI == LT_TI_NULLIDX) break;
			IT::Entry& lit = lex.idTable.table[t.idxTI];
			if (lit.iddatatype == IT::CHAR) {
				char val = ' ';
				if (lit.value.vstr.len > 0) {
                    val = lit.value.vstr.str[0];
				}
				file << "    push " << (int)(unsigned char)val << "\n";
			} else {
				file << "    push " << lit.value.vint << "\n";
			}
			stackDepth++;
			break;
		}
		// идентификаторы
		case LEX_ID: {
			if (t.idxTI == LT_TI_NULLIDX) break;
			if (lex.idTable.table[t.idxTI].idtype == IT::F) {
				string name = lex.idTable.table[t.idxTI].id;
				
				if (isStandardFunc(name)) {
					// ABI: параметры через регистры
					int pCount = getStdFuncParamCount(name);
					if (pCount >= 2) { file << "    pop rsi\n"; stackDepth--; }
					if (pCount >= 1) { file << "    pop rdi\n"; stackDepth--; }
					
					// Выравнивание стека перед вызовом
					if (stackDepth % 2 != 0) file << "    sub rsp, 8\n";
					file << "    call lsa_" << name << "\n";
					if (stackDepth % 2 != 0) file << "    add rsp, 8\n";

					// Очистка верхних байтов для char функций
					if (name == "readch" || name == "toUpper") {
						file << "    movzx rax, al\n";
					}
				} else {
					// Пользовательские функции (cdecl/stdcall hybrid - через стек)
					file << "    call " << name << "\n";
				}
				file << "    push rax\n";
				stackDepth++;
			} else {
				file << "    push qword [" << getMangledName(lex.idTable.table[t.idxTI]) << "]\n";
				stackDepth++;
			}
			break;
		}
		// операции
		case LEX_PLUS: file << "    pop rbx\n    pop rax\n    add rax, rbx\n    push rax\n"; stackDepth--; break;
		case LEX_MINUS: file << "    pop rbx\n    pop rax\n    sub rax, rbx\n    push rax\n"; stackDepth--; break;
		case LEX_STAR: file << "    pop rbx\n    pop rax\n    imul rax, rbx\n    push rax\n"; stackDepth--; break;
		case LEX_COLON: file << "    pop rbx\n    pop rax\n    xor rdx, rdx\n    div rbx\n    push rax\n"; stackDepth--; break;
		case LEX_OST: file << "    pop rbx\n    pop rax\n    xor rdx, rdx\n    div rbx\n    push rdx\n"; stackDepth--; break;
		case LEX_BIT_NOT: file << "    pop rax\n    not rax\n    push rax\n"; break;
		case LEX_MORE: case LEX_LESS: case LEX_ISEQUAL: case LEX_NOT_EQUAL: case LEX_MORE_OR_EQUAL: case LEX_LESS_OR_EQUAL: {
			file << "    pop rbx\n    pop rax\n    cmp rax, rbx\n";
			string setOp;
			switch (t.lexema[0]) {
			case LEX_MORE: setOp = "setg"; break;
			case LEX_LESS: setOp = "setl"; break;
			case LEX_ISEQUAL: setOp = "sete"; break;
			case LEX_NOT_EQUAL: setOp = "setne"; break;
			case LEX_MORE_OR_EQUAL: setOp = "setge"; break;
			case LEX_LESS_OR_EQUAL: setOp = "setle"; break;
			}
			file << "    " << setOp << " al\n    movzx rax, al\n    push rax\n";
			stackDepth--;
			break;
		}
		case LEX_READCH: {
			int uniq = stackDepth++;
			file << "    test rsp, 15\n";
			file << "    jz L_aligned_" << uniq << "\n";
			file << "    sub rsp, 8\n";
			file << "    call lsa_readch\n";
			file << "    add rsp, 8\n";
			file << "    jmp L_end_" << uniq << "\n";
			file << "L_aligned_" << uniq << ":\n";
			file << "    call lsa_readch\n";
			file << "L_end_" << uniq << ":\n";
			file << "    movzx rax, al\n";
			file << "    push rax\n";
			stackDepth++;
			break;
		}
		// инкремент/декремент
		case LEX_INC:
		case LEX_DEC: {
			int k = i - 1;
			while (k >= start && lex.lexTable.table[k].lexema[0] == '\0') k--;
			if (k >= start && lex.lexTable.table[k].lexema[0] == LEX_ID && lex.lexTable.table[k].idxTI != LT_TI_NULLIDX) {
				IT::Entry& varEntry = lex.idTable.table[lex.lexTable.table[k].idxTI];
				file << "    pop rax\n";
				stackDepth--;
				string op = (t.lexema[0] == LEX_INC) ? "inc" : "dec";
				file << "    " << op << " qword [" << getMangledName(varEntry) << "]\n";
			}
			break;
		}
		}
	}
}

// определение типа выражения
IT::IDDATATYPE getExprType(int start, int end, Lex::LEX& lex) {
	IT::IDDATATYPE type = IT::CHAR;
	bool hasLogic = false;
	bool hasUnsigned = false;
	
	for (int i = start; i < end; i++) {
		if (lex.lexTable.table[i].lexema[0] == LEX_ID || lex.lexTable.table[i].lexema[0] == LEX_LITERAL) {
			int idx = lex.lexTable.table[i].idxTI;
			if (idx != LT_TI_NULLIDX) {
				IT::IDDATATYPE t = lex.idTable.table[idx].iddatatype;
				if (t == IT::UNSIGNED) hasUnsigned = true;
				if (t == IT::LOGIC) hasLogic = true;
			}
		}
		else if (lex.lexTable.table[i].lexema[0] == LEX_PLUS ||
			lex.lexTable.table[i].lexema[0] == LEX_MINUS ||
			lex.lexTable.table[i].lexema[0] == LEX_STAR ||
			lex.lexTable.table[i].lexema[0] == LEX_DIRSLASH ||
			lex.lexTable.table[i].lexema[0] == LEX_OST ||
			lex.lexTable.table[i].lexema[0] == LEX_COLON) {
			hasUnsigned = true;
		}
	}

	// Проверяем наличие операторов сравнения, которые всегда возвращают LOGIC
	for (int i = start; i < end; i++) {
		if (lex.lexTable.table[i].lexema[0] == LEX_MORE ||
			lex.lexTable.table[i].lexema[0] == LEX_LESS ||
			lex.lexTable.table[i].lexema[0] == LEX_ISEQUAL ||
			lex.lexTable.table[i].lexema[0] == LEX_NOT_EQUAL ||
			lex.lexTable.table[i].lexema[0] == LEX_MORE_OR_EQUAL ||
			lex.lexTable.table[i].lexema[0] == LEX_LESS_OR_EQUAL) {
			type = IT::LOGIC;
			return type; // Сравнение всегда дает логический результат
		}
	}
	
	if (hasUnsigned) type = IT::UNSIGNED;
	else if (hasLogic) type = IT::LOGIC;
	
	return type;
}

// генерация кода
void asmGenerator(Lex::LEX& lex, wchar_t outfile[]) {
	// подготовка файла
	char narrow_outfile[300];
	wcstombs(narrow_outfile, outfile, 300);
	ofstream file(narrow_outfile);
	if (!file.is_open()) throw ERROR_THROW(13);

	file << ASM_HEAD;
	
	// Комментарий для сборки
	file << "; Ссылка на библиотеку: " << LIB_PATH << "\n";
	file << "; Пример сборки: g++ -o program program.o " << LIB_PATH << " -no-pie\n\n";

	// секция .bss для неинициализированных данных
	file << "section .bss\n";
	for (int i = 0; i < lex.idTable.size; i++) {
		IT::Entry& entry = lex.idTable.table[i];
		if (entry.idtype == IT::V || entry.idtype == IT::P) {
			file << "    " << getMangledName(entry) << " resq 1\n";
		}
	}
	file << "\nsection .text\n";

	stack<LabelBlock> blockStack;
	int labelCounter = 0;

	// главный цикл по таблице лексем
	for (int i = 0; i < lex.lexTable.size; i++) {
		LT::Entry& t = lex.lexTable.table[i];
		if (t.lexema[0] == '\0') continue;

		switch (t.lexema[0]) {
		// точка входа
		case LEX_MAIN: {
			file << "\nmain:\n    push rbp\n    mov rbp, rsp\n";
			blockStack.push({ 3, 0, 0, 0 });
			break;
		}
		// объявление функции
		case LEX_FUNC: {
			int idIdx = lex.lexTable.table[i + 2].idxTI;
			string funcName = lex.idTable.table[idIdx].id;
			file << "\n" << funcName << ":\n    push rbp\n    mov rbp, rsp\n";
			// копирование параметров из стека в переменные
			int k = i + 4;
			int paramOffset = 16;
			int paramCount = 0;
			while (lex.lexTable.table[k].lexema[0] != LEX_RIGHTTHESIS) {
				if (lex.lexTable.table[k].lexema[0] == LEX_ID) {
					IT::Entry& pEntry = lex.idTable.table[lex.lexTable.table[k].idxTI];
					file << "    mov rax, [rbp + " << paramOffset << "]\n";
					file << "    mov [" << getMangledName(pEntry) << "], rax\n";
					paramOffset += 8;
					paramCount++;
				}
				k++;
			}
			blockStack.push({ 3, 0, 0, paramCount * 8 });
			i = k;
			break;
		}
		// возврат из функции
		case LEX_SEND: {
			int endExpr = i + 1;
			while (endExpr < lex.lexTable.size && lex.lexTable.table[endExpr].lexema[0] != LEX_SEMICOLON) endExpr++;
			processExpression(i + 1, endExpr, lex, file);
			int retBytes = 0;
			if (!blockStack.empty()) {
				stack<LabelBlock> temp = blockStack;
				while (!temp.empty()) {
					if (temp.top().type == 3) { retBytes = temp.top().paramSize; break; }
					temp.pop();
				}
			}
			file << "    pop rax\n    mov rsp, rbp\n    pop rbp\n";
			if (retBytes > 0) file << "    ret " << retBytes << "\n";
			else file << "    ret\n";
			i = endExpr;
			break;
		}
		// вызов writech
		case LEX_WRITECH: {
			int j = i + 1;
			while (j < lex.lexTable.size && lex.lexTable.table[j].lexema[0] == '\0') j++;
			if (j < lex.lexTable.size && lex.lexTable.table[j].lexema[0] ==LEX_LEFTTHESIS) {
				int startExpr = j + 1;
				int endExpr = startExpr;
				int balance = 1;
				while (endExpr < lex.lexTable.size) {
					if (lex.lexTable.table[endExpr].lexema[0] == LEX_LEFTTHESIS) balance++;
					if (lex.lexTable.table[endExpr].lexema[0] == LEX_RIGHTTHESIS) balance--;
					if (balance == 0) break;
					endExpr++;
				}
				processExpression(startExpr, endExpr, lex, file);
				
				file << "    pop rdi\n"; // Аргумент в RDI
				
				IT::IDDATATYPE type = getExprType(startExpr, endExpr, lex);
				
				int uniq = labelCounter++;
				file << "    test rsp, 15\n";
				file << "    jz L_aligned_" << uniq << "\n";
				file << "    sub rsp, 8\n";
				
				if (type == IT::UNSIGNED) {
					file << "    call lsa_writeuint\n";
				} else if (type == IT::LOGIC) {
					file << "    call lsa_writelogic\n";
				} else {
					file << "    call lsa_writech\n";
				}
				
				file << "    add rsp, 8\n";
				file << "    jmp L_end_" << uniq << "\n";
				file << "L_aligned_" << uniq << ":\n";

				if (type == IT::UNSIGNED) {
					file << "    call lsa_writeuint\n";
				} else if (type == IT::LOGIC) {
					file << "    call lsa_writelogic\n";
				} else {
					file << "    call lsa_writech\n";
				}
				
				file << "L_end_" << uniq << ":\n";
				
				i = endExpr;
			}
			break;
		}
		// присваивание или вызов функции как инструкция
		case LEX_ID: {
			if (i + 1 < lex.lexTable.size && lex.lexTable.table[i+1].lexema[0] == LEX_EQUAL) {
				IT::Entry& varEntry = lex.idTable.table[t.idxTI];
				int endStmt = i + 2;
				while (endStmt < lex.lexTable.size && lex.lexTable.table[endStmt].lexema[0] != LEX_SEMICOLON) endStmt++;
				processExpression(i + 2, endStmt, lex, file);
				file << "    pop rax\n";
				file << "    mov [" << getMangledName(varEntry) << "], rax\n";
				i = endStmt;
			} else {
				// выражение, результат которого не используется
				int endStmt = i + 1;
				while (endStmt < lex.lexTable.size && lex.lexTable.table[endStmt].lexema[0] != LEX_SEMICOLON) endStmt++;
				processExpression(i, endStmt, lex, file);
				file << "    pop rax ; clear stack\n";
				i = endStmt;
			}
			break;
		}
		// выражение, результат которого не используется
		case LEX_LITERAL:
		case LEX_READCH:
		case LEX_BIT_NOT:
		case LEX_INC:
		case LEX_DEC: {
			int endStmt = i + 1;
			while (endStmt < lex.lexTable.size && lex.lexTable.table[endStmt].lexema[0] != LEX_SEMICOLON) endStmt++;
			processExpression(i, endStmt, lex, file);
			file << "    pop rax ; clear stack\n";
			i = endStmt;
			break;
		}
		// условный оператор
		case LEX_IF: {
			int l_else = labelCounter++;
			int l_end = labelCounter++;
			blockStack.push({ 0, l_else, l_end, 0 });
			// генерация кода для условия
			int j = i + 2;
			int balance = 1;
			int endCond = j;
			while (endCond < lex.lexTable.size) {
				if (lex.lexTable.table[endCond].lexema[0] == LEX_LEFTTHESIS) balance++;
				if (lex.lexTable.table[endCond].lexema[0] == LEX_RIGHTTHESIS) balance--;
				if (balance == 0) break;
				endCond++;
			}
			processExpression(j, endCond, lex, file);
			i = endCond;
			break;
		}
		// переход в блок else
		case LEX_DIFFER: {
			if (!blockStack.empty()) blockStack.top().type = 1;
			break;
		}
		// цикл
		case LEX_BECAUSE: {
			int l_start = labelCounter++;
			int l_end = labelCounter++;
			blockStack.push({ 2, l_start, l_end, 0 });
			// блок инициализации
			int j = i + 2;
			int endInit = j;
			while (endInit < lex.lexTable.size && lex.lexTable.table[endInit].lexema[0] != LEX_SEMICOLON) endInit++;
			
			// Проверка на присваивание: i = 0
			if (lex.lexTable.table[j].lexema[0] == LEX_ID && lex.lexTable.table[j+1].lexema[0] == LEX_EQUAL) {
				IT::Entry& varEntry = lex.idTable.table[lex.lexTable.table[j].idxTI];
				processExpression(j + 2, endInit, lex, file);
				file << "    pop rax\n    mov [" << getMangledName(varEntry) << "], rax\n";
			} 
			// Проверка на объявление: unsigned integer i = 0
			else if ((lex.lexTable.table[j].lexema[0] == LEX_UNSIGNED_INTEGER || 
					  lex.lexTable.table[j].lexema[0] == LEX_CHAR || 
					  lex.lexTable.table[j].lexema[0] == LEX_LOGIC) &&
					  lex.lexTable.table[j+1].lexema[0] == LEX_ID && 
					  lex.lexTable.table[j+2].lexema[0] == LEX_EQUAL) {
				IT::Entry& varEntry = lex.idTable.table[lex.lexTable.table[j+1].idxTI];
				processExpression(j + 3, endInit, lex, file);
				file << "    pop rax\n    mov [" << getMangledName(varEntry) << "], rax\n";
			}
			else {
				processExpression(j, endInit, lex, file);
				if (endInit > j) file << "    pop rax ; clear stack init\n";
			}
			// метка начала цикла
			file << "L" << l_start << ":\n";
			// блок условия
			int startCond = endInit + 1;
			int endCond = startCond;
			while (endCond < lex.lexTable.size && lex.lexTable.table[endCond].lexema[0] !=LEX_SEMICOLON) endCond++;
			processExpression(startCond, endCond, lex, file);

			int startStep = endCond + 1;
			int endStep = startStep;
			while (endStep < lex.lexTable.size && lex.lexTable.table[endStep].lexema[0] != LEX_RIGHTTHESIS) endStep++;
			i = endStep;
			break;
		}
		// начало блока
		case LEX_LEFTBRACE: {
			if (!blockStack.empty()) {
				LabelBlock& b = blockStack.top();
				// условный переход для if
				if (b.type == 0) {
					file << "    pop rax\n    cmp rax, 0\n    je L" << b.label_1 << "\n";
				// условный переход для because
				} else if (b.type == 2) {
					file << "    pop rax\n    cmp rax, 0\n    je L" << b.label_2 << "\n";
				}
			}
			break;
		}
		// конец блока
		case LEX_BRACELET: {
			if (!blockStack.empty()) {
				LabelBlock b = blockStack.top();
				blockStack.pop();
				// конец блока if
				if (b.type == 0) {
					int next = i + 1;
					while (next < lex.lexTable.size && (lex.lexTable.table[next].lexema[0] == '\0' || lex.lexTable.table[next].lexema[0] == LEX_SEMICOLON)) next++;
					// если есть differ, переходим к его обработке
					if (next < lex.lexTable.size && lex.lexTable.table[next].lexema[0] == LEX_DIFFER) {
						file << "    jmp L" << b.label_2 << "\nL" << b.label_1 << ":\n";
						b.type = 1;
						blockStack.push(b);
					// если нет differ, ставим метку конца
					} else {
						file << "L" << b.label_1 << ":\n";
					}
				}
				// конец блока else
				else if (b.type == 1) {
					file << "L" << b.label_2 << ":\n";
				}
				// конец блока because
				else if (b.type == 2) {
					// генерация кода для шага цикла
					int back = i;
					int braces = 0;
					while (back > 0) {
						if (lex.lexTable.table[back].lexema[0] == LEX_BRACELET) braces++;
						if (lex.lexTable.table[back].lexema[0] == LEX_LEFTBRACE) braces--;
						if (braces == 0) break;
						back--;
					}
					int stepEnd = back - 1;
					int stepStart = stepEnd;
					while (stepStart > 0 && lex.lexTable.table[stepStart].lexema[0] != LEX_SEMICOLON) stepStart--;
					stepStart++;

					bool isAssign = false;
					for(int k=stepStart; k<stepEnd; k++) if(lex.lexTable.table[k].lexema[0]==LEX_EQUAL) isAssign=true;

					if (isAssign) {
						IT::Entry& varEntry = lex.idTable.table[lex.lexTable.table[stepStart].idxTI];
						processExpression(stepStart+2, stepEnd, lex, file);
						file << "    pop rax\n    mov [" << getMangledName(varEntry) << "], rax\n";
					} else {
						processExpression(stepStart, stepEnd, lex, file);
					}
					// переход на новую итерацию
					file << "    jmp L" << b.label_1 << "\nL" << b.label_2 << ":\n";
				}
				// конец блока функции
				else if (b.type == 3) {
					file << "    pop rbp\n";
					if (b.paramSize > 0) file << "    ret " << b.paramSize << "\n";
					else file << "    ret\n";
				}
			}
			break;
		}
		}
	}
	file.close();
}
}