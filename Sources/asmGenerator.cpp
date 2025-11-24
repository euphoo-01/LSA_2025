#include "../Headers/asmGenerator.h"
#include "cstring"
#define PODCHERK '_'
#define FUNC "func"
#define LOCAL "local"
#define IFI "ifi"
#define ELSEI "elsei"
#define UNTILI "while"

namespace ASMGenerator {
	void asmGenerator(LexA::LEX lex, wchar_t outfile[]) { // было wchar_t
		int filenameLen = wcslen(outfile);
		char filename[filenameLen];
		wcstombs(filename, outfile, filenameLen);
		ofstream asmout(filename);

		asmout << HEAD;
		asmout << PROTOS;

		for (int i = 0; i < lex.idTable.size; i++) {
			for (int j = 0; j < lex.idTable.size; j++) {
				if (i == j) {
					continue;
				}
				if (lex.idTable.table[i].idtype == IT::F) {
					continue;
				}
				if (strcmp(lex.idTable.table[i].id, lex.idTable.table[j].id) == 0) {
					size_t length = strlen(lex.idTable.table[i].id);

					lex.idTable.table[i].id[length] = PODCHERK;
					lex.idTable.table[i].id[length + 1] = '\0';
					j = -1;
				}
			}
		}
		asmout << ".CONST\n";
		asmout << "\tnull_division BYTE \'ERROR: DIVISION BY ZERO\', 0\n";
		asmout << "\terror_read BYTE \'ERROR: READING ERROR\', 0\n";
		asmout << "\tTRUE BYTE \'true\', 0\n";
		asmout << "\tFALSE BYTE \'false\', 0\n";
		for (int i = 0; i < lex.idTable.size; i++) {
			if (lex.idTable.table[i].idtype == IT::L) {
				if (lex.idTable.table[i].iddatatype == IT::UINT) {
					asmout << "\t" << lex.idTable.table[i].id << " DWORD " << (unsigned int)lex.idTable.table[i].value.vint << '\n';
				}
				else if (lex.idTable.table[i].iddatatype == IT::DOUBLE) {
					asmout << "\t" << lex.idTable.table[i].id << " real8 " << (double)lex.idTable.table[i].value.vint;
					if ((double)(int)lex.idTable.table[i].value.vint == lex.idTable.table[i].value.vint) {
						asmout << ".0";
					}
					asmout << '\n';
				}
				else if (lex.idTable.table[i].iddatatype == IT::STR) {
					asmout << "\t" << lex.idTable.table[i].id << " BYTE ";
					for (int k = 0; k < lex.idTable.table[i].value.vstr->len; k++) {
						if (lex.idTable.table[i].value.vstr->str[k] == MARK) {
							asmout << MARK;
						}
						else {
							asmout << lex.idTable.table[i].value.vstr->str[k];
						}
					}
					asmout << ", 0\n";
				}
				else if (lex.idTable.table[i].iddatatype == IT::BOOL) {
					asmout << "\t" << lex.idTable.table[i].id << " DWORD " << static_cast<int>(lex.idTable.table[i].value.vint) << "\n";
				}
			}
		}
		asmout << "\n.DATA\n";
		for (int i = 0; i < lex.idTable.size; i++) {
			if (lex.idTable.table[i].idtype == IT::V) {
				if (lex.idTable.table[i].iddatatype == IT::BOOL) {
					asmout << "\t" << lex.idTable.table[i].id << " DWORD 0\n";
				}
				else if (lex.idTable.table[i].iddatatype == IT::UINT) {
					asmout << "\t" << lex.idTable.table[i].id << " DWORD 0\n";
				}
				else if (lex.idTable.table[i].iddatatype == IT::DOUBLE) {
					asmout << "\t" << lex.idTable.table[i].id << " real8 0.0\n";
				}
				else if (lex.idTable.table[i].iddatatype == IT::STR) {
					asmout << "\t" << lex.idTable.table[i].id << " BYTE 128 dup(0)\n";
				}
			}
			else if (lex.idTable.table[i].idtype == IT::P && lex.idTable.table[i].iddatatype == IT::STR) {
				asmout << "\t" << lex.idTable.table[i].id << " BYTE 128 dup(0)\n";
			}
		}
		stack<pair<string, int>> parms; // стек для парметров функции и типов
		map<string, vector<string>> funcwithSTR; // функции со строковыми параметрами
		stack<string> STRp; // для названий переменных строкового типа в функции
		stack<string> STRs; // для названий переменных стркового типа, которые являются ресурсами
		stack<string> OSTPARMS; // остальные параметры в функции
		stack<string> stk; // стек для отслеживания текущего блока(функция, условный, цикл)
		stack<int> ifi; // стек для отслеживания текущего номера условия
		stack<int> whilei;// стек для отслеживания текущего номера цикла
		int num_of_ret = 0,
			num_of_logical = 0,
			num_of_if = 0,
			num_of_while = 0,
			num_of_func = 0;
		string namefunc = "";
		bool funcFlag = false,
			mainFlag = false,
			flag_return = false,
			flag_else = false,
			flag_if = false,
			flag_while = false;
		asmout << "\n\n.CODE\npush 1251d\ncall SetConsoleOutputCP\npush 1251d\ncall SetConsoleCP\n\n";
		asmout << END << '\n';
		for (int i = 0; i < lex.lexTable.size; i++)
		{
			switch (lex.lexTable.table[i].lexema[0])
			{
			case LEX_FUNCTION:
			{
				namefunc = lex.idTable.table[lex.lexTable.table[i += 2].idxTI].id;
				string buf = FUNC + to_string(num_of_func);
				if (lex.idTable.table[lex.lexTable.table[i].idxTI].iddatatype == IT::DOUBLE) {
					buf += 'r';
				}
				num_of_func++;
				stk.push(buf);
				asmout << namefunc << " PROC ";
				int index = i;
				while (lex.lexTable.table[index].lexema[0] != LEX_RIGHTTHESIS) {
					index++;
				}

				i = index;
				while (lex.lexTable.table[index].lexema[0] != LEX_LEFTTHESIS)
				{
					if (lex.lexTable.table[index].lexema[0] == LEX_COMMA)
					{
						asmout << ", ";
					}
					if (lex.lexTable.table[index].lexema[0] == LEX_ID) {
						if (lex.idTable.table[lex.lexTable.table[index].idxTI].idtype == IT::P)
						{
							if (lex.idTable.table[lex.lexTable.table[index].idxTI].iddatatype == IT::DOUBLE)
							{
								asmout << lex.idTable.table[lex.lexTable.table[index].idxTI].id << " : ";
								asmout << "real8";
							}
							else if (lex.idTable.table[lex.lexTable.table[index].idxTI].iddatatype == IT::UINT)
							{
								asmout << lex.idTable.table[lex.lexTable.table[index].idxTI].id << " : ";
								asmout << "DWORD";
							}
							else if (lex.idTable.table[lex.lexTable.table[index].idxTI].iddatatype == IT::BOOL)
							{
								asmout << lex.idTable.table[lex.lexTable.table[index].idxTI].id << " : ";
								asmout << "DWORD";
							}
							else if (lex.idTable.table[lex.lexTable.table[index].idxTI].iddatatype == IT::STR)
							{
								funcwithSTR[namefunc].push_back(lex.idTable.table[lex.lexTable.table[index].idxTI].id);
							}
						}
					}
					index--;
				}
				funcFlag = true;
				asmout << '\n';
				break;
			}
			case LEX_MAIN:
			{
				stk.push("main");
				mainFlag = true;
				asmout << "main PROC\n";
				break;
			}
			case LEX_EQUAL:
			{
				int respos = i - 1;
				while (lex.lexTable.table[i].lexema[0] != LEX_SEMICOLON)
				{
					switch (lex.lexTable.table[i].lexema[0])
					{
					case LEX_LITERAL:
					{
						if (lex.idTable.table[lex.lexTable.table[i].idxTI].iddatatype == IT::IDDATATYPE::STR)
						{
							parms.push(make_pair(lex.idTable.table[lex.lexTable.table[i].idxTI].id, 4));
						}
						else if (lex.idTable.table[lex.lexTable.table[i].idxTI].iddatatype == IT::IDDATATYPE::DOUBLE)
						{
							parms.push(make_pair(lex.idTable.table[lex.lexTable.table[i].idxTI].id, 2));
						}
						else if(lex.idTable.table[lex.lexTable.table[i].idxTI].iddatatype == IT::IDDATATYPE::UINT)
						{
							parms.push(make_pair(lex.idTable.table[lex.lexTable.table[i].idxTI].id, 1));
						}
						else {
							parms.push(make_pair(lex.idTable.table[lex.lexTable.table[i].idxTI].id, 3));
						}
						break;
					}
					case LEX_ID:
					{
						if (lex.idTable.table[lex.lexTable.table[i].idxTI].idtype == IT::IDTYPE::F && lex.idTable.table[lex.lexTable.table[i].idxTI].iddatatype == IT::IDDATATYPE::DOUBLE)
						{
							int kol = lex.functions[lex.idTable.table[lex.lexTable.table[i].idxTI].id].size();
							if (funcwithSTR.contains(lex.idTable.table[lex.lexTable.table[i].idxTI].id)) {
								auto o = funcwithSTR[lex.idTable.table[lex.lexTable.table[i].idxTI].id];
								for (int i = 0; i < o.size(); i++) {
									STRp.push(o[i]);
								}
							}
							for (int i = 0; i < kol - 1; i++) {	
								pair<string, int> buf = parms.top();
								parms.pop();
								if (buf.second != 4) {
									OSTPARMS.push(buf.first);
								}
								else{
									STRs.push(buf.first);
								}
							}
							pair<string, int> buf = parms.top();
							parms.pop();
							if(buf.second != 4){
								OSTPARMS.push(buf.first);
							}
							else {
								STRs.push(buf.first);
							}
							while (!STRp.empty()) {
								asmout << "\tlea esi, [" << STRs.top() << "]\n";
								asmout << "\tlea edi, [" << STRp.top() << "]\n";
								asmout << "\tmov ecx, lengthof " << STRs.top() << '\n';
								asmout << "\trep movsb" << '\n';
								STRp.pop();
								STRs.pop();
							}
							asmout << "\tINVOKE " << lex.idTable.table[lex.lexTable.table[i].idxTI].id;
							while (!OSTPARMS.empty()) {
								asmout << ", " << OSTPARMS.top();
								OSTPARMS.pop();
							}
							asmout << '\n';
						}
						else if (lex.idTable.table[lex.lexTable.table[i].idxTI].idtype == IT::IDTYPE::F)
						{
							if (strcmp(lex.idTable.table[lex.lexTable.table[i].idxTI].id, "compare") == 0) {
								asmout << "\tINVOKE " << lex.idTable.table[lex.lexTable.table[i].idxTI].id;
								string buf = parms.top().first;
								parms.pop();
								asmout << ", addr " << parms.top().first;
								parms.pop();
								asmout << ", addr " << buf;
								asmout << '\n';
								asmout << "\tpush eax\n";
								i++;
								continue;
							}
							if (strcmp(lex.idTable.table[lex.lexTable.table[i].idxTI].id, "len") == 0) {
								asmout << "\tINVOKE " << lex.idTable.table[lex.lexTable.table[i].idxTI].id;
								asmout << ", addr " << parms.top().first;
								parms.pop();
								asmout << '\n';
								asmout << "\tpush eax\n";
								i++;
								continue;
							}
							int kol = lex.functions[lex.idTable.table[lex.lexTable.table[i].idxTI].id].size();
							if (funcwithSTR.contains(lex.idTable.table[lex.lexTable.table[i].idxTI].id)) {
								auto o = funcwithSTR[lex.idTable.table[lex.lexTable.table[i].idxTI].id];
								for (int i = 0; i < o.size(); i++) {
									STRp.push(o[i]);
								}
							}
							for (int i = 0; i < kol - 1; i++) {
								pair<string, int> buf = parms.top();
								parms.pop();
								if (buf.second != 4) {
									OSTPARMS.push(buf.first);
								}
								else {
									STRs.push(buf.first);
								}
							}
							pair<string, int> buf = parms.top();
							parms.pop();
							if (buf.second != 4) {
								OSTPARMS.push(buf.first);
							}
							else {
								STRs.push(buf.first);
							}
							while (!STRp.empty()) {
								asmout << "\tlea esi, [" << STRs.top() << "]\n";
								asmout << "\tlea edi, [" << STRp.top() << "]\n";
								asmout << "\tmov ecx, lengthof " << STRs.top() << '\n';
								asmout << "\trep movsb" << '\n';
								STRp.pop();
								STRs.pop();
							}
							asmout << "\tINVOKE " << lex.idTable.table[lex.lexTable.table[i].idxTI].id;
							while (!OSTPARMS.empty()) {
								asmout << ", " << OSTPARMS.top();
								OSTPARMS.pop();
							}
							asmout << "\n\tpush eax\n";
						}
						else if(lex.idTable.table[lex.lexTable.table[i].idxTI].iddatatype == IT::IDDATATYPE::DOUBLE)
						{
							parms.push(make_pair(lex.idTable.table[lex.lexTable.table[i].idxTI].id, 2));
						}
						else if (lex.idTable.table[lex.lexTable.table[i].idxTI].iddatatype == IT::IDDATATYPE::UINT)
						{
							parms.push(make_pair(lex.idTable.table[lex.lexTable.table[i].idxTI].id, 1));
						}
						else if (lex.idTable.table[lex.lexTable.table[i].idxTI].iddatatype == IT::IDDATATYPE::STR)
						{
							parms.push(make_pair(lex.idTable.table[lex.lexTable.table[i].idxTI].id, 4));
						}
						else if (lex.idTable.table[lex.lexTable.table[i].idxTI].iddatatype == IT::IDDATATYPE::BOOL)
						{
							parms.push(make_pair(lex.idTable.table[lex.lexTable.table[i].idxTI].id, 3));
						}
						break;
					}
					case LEX_STAR:
					{
						if (lex.idTable.table[lex.lexTable.table[respos].idxTI].iddatatype == IT::IDDATATYPE::DOUBLE)
						{
							if (!parms.empty()) {
								if (parms.top().second == 2) {
									asmout << "\tfld " << parms.top().first << '\n';
									parms.pop();
								}
							}
							if (!parms.empty()) {
								if (parms.top().second == 2) {
									asmout << "\tfld " << parms.top().first << '\n';
									parms.pop();
								}
							}
							asmout << "\tfmul\n"; 
						}
						else
						{
							if (!parms.empty()) {
								if (parms.top().second == 1) {
									asmout << "\tpush " << parms.top().first << '\n';
									parms.pop();
								}
							}
							if (!parms.empty()) {
								if (parms.top().second == 1) {
									asmout << "\tpush " << parms.top().first << '\n';
									parms.pop();
								}
							}
							asmout << "\tpop eax\n\tpop ebx\n";
							asmout << "\tmul ebx\n\tpush eax\n";
						}
						break;
					}
					case LEX_PLUS:
					{
						if (lex.idTable.table[lex.lexTable.table[respos].idxTI].iddatatype == IT::IDDATATYPE::DOUBLE)
						{
							if (!parms.empty()) {
								if (parms.top().second == 2) {
									asmout << "\tfld " << parms.top().first << '\n';
									parms.pop();
								}
							}
							if (!parms.empty()) {
								if (parms.top().second == 2) {
									asmout << "\tfld " << parms.top().first << '\n';
									parms.pop();
								}
							}
							asmout << "\tfadd\n";
						}
						else
						{
							if (!parms.empty()) {
								if (parms.top().second == 1) {
									asmout << "\tpush " << parms.top().first << '\n';
									parms.pop();
								}
							}
							if (!parms.empty()) {
								if (parms.top().second == 1) {
									asmout << "\tpush " << parms.top().first << '\n';
									parms.pop();
								}
							}
							asmout << "\tpop eax\n\tpop ebx\n";
							asmout << "\tadd eax, ebx\n\tpush eax\n";
						}
						break;
					}
					case LEX_MINUS:
					{
						if (lex.idTable.table[lex.lexTable.table[respos].idxTI].iddatatype == IT::IDDATATYPE::DOUBLE)
						{
							if (!parms.empty()) {
								if (parms.top().second == 2) {
									asmout << "\tfld " << parms.top().first << '\n';
									parms.pop();
								}
							}
							if (!parms.empty()) {
								if (parms.top().second == 2) {
									asmout << "\tfld " << parms.top().first << '\n';
									parms.pop();
								}
							}
							asmout << "\tfsub\n"; 
						}
						else
						{
							if (!parms.empty()) {
								if (parms.top().second == 1) {
									asmout << "\tpush " << parms.top().first << '\n';
									parms.pop();
								}
							}
							if (!parms.empty()) {
								if (parms.top().second == 1) {
									asmout << "\tpush " << parms.top().first << '\n';
									parms.pop();
								}
							}
							asmout << "\tpop eax\n\tpop ebx\n";
							asmout << "\tsub eax, ebx\n\tpush eax\n";
						}
						break;
					}
					case LEX_DIRSLASH:
					{
						if (lex.idTable.table[lex.lexTable.table[respos].idxTI].iddatatype == IT::IDDATATYPE::DOUBLE)
						{
							if (!parms.empty()) {
								if (parms.top().second == 2) {
									asmout << "\tfld " << parms.top().first << '\n';
									parms.pop();
								}
							}
							if (!parms.empty()) {
								if (parms.top().second == 2) {
									asmout << "\tfld " << parms.top().first << '\n';
									parms.pop();
								}
							}
							asmout << "\tfldz\n"; 
							asmout << "\tfcomip st(0), st(1)\n"; 
							asmout << "\tje SOMETHINGWRONG\n"; 
							asmout << "\tfdiv\n"; 
						}
						else
						{
							if (!parms.empty()) {
								if (parms.top().second == 1) {
									asmout << "\tpush " << parms.top().first << '\n';
									parms.pop();
								}
							}
							if (!parms.empty()) {
								if (parms.top().second == 1) {
									asmout << "\tpush " << parms.top().first << '\n';
									parms.pop();
								}
							}
							asmout << "\tpop eax\n\tpop ebx\n";
							asmout << "\tcmp ebx, 0\n\tje SOMETHINGWRONG\n";
							asmout << "\tcdq\n";
							asmout << "\tidiv ebx\n\tpush eax\n"; 
						}
						break;
					}
					case LEX_OST: 
					{
						if (!parms.empty()) {
							if (parms.top().second == 1) {
								asmout << "\tpush " << parms.top().first << '\n';
								parms.pop();
							}
						}
						if (!parms.empty()) {
							if (parms.top().second == 1) {
								asmout << "\tpush " << parms.top().first << '\n';
								parms.pop();
							}
						}
						asmout << "\tpop eax\n\tpop ebx\n";
						asmout << "\tcmp ebx, 0\n\tje SOMETHINGWRONG\n";
						asmout << "\tcdq\n";
						asmout << "\tidiv ebx\n\tpush edx\n"; // Остаток помещается в edx
						break;
					}
					case LEX_AND:
					{
						if (!parms.empty()) {
							if (parms.top().second == 1) {
								asmout << "\tpush " << parms.top().first << '\n';
								parms.pop();
							}
						}
						if (!parms.empty()) {
							if (parms.top().second == 1) {
								asmout << "\tpush " << parms.top().first << '\n';
								parms.pop();
							}
						}
						asmout << "\tpop ebx\n";  
						asmout << "\tpop eax\n";  
						asmout << "\tand eax, ebx\n"; 
						asmout << "\tpush eax\n"; 
						break;
					}

					case LEX_OR:
					{
						if (!parms.empty()) {
							if (parms.top().second == 1) {
								asmout << "\tpush " << parms.top().first << '\n';
								parms.pop();
							}
						}
						if (!parms.empty()) {
							if (parms.top().second == 1) {
								asmout << "\tpush " << parms.top().first << '\n';
								parms.pop();
							}
						}
						asmout << "\tpop ebx\n";  
						asmout << "\tpop eax\n";  
						asmout << "\tor eax, ebx\n"; 
						asmout << "\tpush eax\n";
						break;
					}
					default:
						break;
					}
					i++;
				}
				if (lex.idTable.table[lex.lexTable.table[respos].idxTI].iddatatype == IT::IDDATATYPE::DOUBLE) {
					if (!parms.empty()) {
						asmout << "\tfld " << parms.top().first << '\n';
						parms.pop();
					}
					asmout << "\tfstp " << lex.idTable.table[lex.lexTable.table[respos].idxTI].id << "\n\n";
				}
				else if (lex.idTable.table[lex.lexTable.table[respos].idxTI].iddatatype == IT::IDDATATYPE::STR) {
					if (parms.top().second == 4) {
						if (!parms.empty()) {
							asmout << "\tlea esi, [" << parms.top().first << "]\n";
							asmout << "\tlea edi, [" << lex.idTable.table[lex.lexTable.table[respos].idxTI].id << "]\n";
							asmout << "\tmov ecx, lengthof " << parms.top().first << "\n";
							asmout << "\trep movsb\n";
							parms.pop();
						}
					}
				}
				else {
							if (!parms.empty()) {
								asmout << "\tpush " << parms.top().first << '\n';
								parms.pop();
							}
							asmout << "\tpop " << lex.idTable.table[lex.lexTable.table[respos].idxTI].id << "\n\n";
						}
						break;
					}
			case LEX_RETURN:
			{
				i++;
				if (lex.idTable.table[lex.lexTable.table[i].idxTI].iddatatype == IT::IDDATATYPE::DOUBLE)
					{
						asmout << "\tfld ";
						asmout << lex.idTable.table[lex.lexTable.table[i++].idxTI].id << '\n';
					}
					else {
							asmout << "\tpush ";
							asmout << lex.idTable.table[lex.lexTable.table[i++].idxTI].id << '\n';
					}
				if (funcFlag)
				{
					asmout << "\tjmp local" << num_of_ret << endl;
					flag_return = true;
				}
				if (mainFlag)
				{
					asmout << "\tjmp theend\n";
					flag_return = true;
				}
				break;
			}
			case LEX_BRACELET:
			{
				string buf = stk.top();
				stk.pop();
				if (buf == "main") {
					if (flag_return)
					{
						//asmout << "theend:\n";
						flag_return = false;
					}
					asmout << FOOTER;
					break;
				}
				if (buf.find(FUNC) != string::npos)
				{
					if (flag_return)
					{
						asmout << LOCAL << num_of_ret++ << ":\n";
						if (buf.find("r") != string::npos) {
							asmout << "\t\n\tret\n";
						}
						else {
							asmout << "\tpop eax\n\tret\n";
						}
						flag_return = false;
					}
					asmout << namefunc << " ENDP\n\n";
					funcFlag = false;
				}
				if (buf.find(IF) != string::npos)
				{
					if (lex.lexTable.table[i + 2].lexema[0] == LEX_ELSE) {
						asmout << "\tjmp ifEnd" << ifi.top() << "\n";
						asmout << ELSEI << ifi.top() << ":\n";
					}
					else {
						asmout << ELSEI << ifi.top() << ":\n";
						ifi.pop();
					}
				}
				if (buf.find(ELSEI) != string::npos)
				{
						asmout << "ifEnd" << ifi.top() << ":\n";
						ifi.pop();
				}
				if (buf.find(UNTILI) != string::npos)
				{
					asmout << "jmp while" << whilei.top() << '\n';
					asmout << "notwhile" << whilei.top() << ":\n";
					whilei.pop();
				}
				break;
			}
			case LEX_LEFTBRACE:
				if (flag_if)
				{
					asmout << IFI << ifi.top() << ":\n";
					flag_if = false;
				}
				if (flag_else) {
					flag_else = false;
				}
				if (flag_while)
				{
					asmout << UNTILI << whilei.top() << ":\n";
					flag_while = false;
					asmout << "\tmov al, \'" << lex.lexTable.table[i - 3].lexema[0] << "\'\n";
					asmout << "\tpush eax\n";
					if (lex.idTable.table[lex.lexTable.table[i - 2].idxTI].idtype == IT::P && lex.idTable.table[lex.lexTable.table[i - 2].idxTI].iddatatype != IT::DOUBLE) {
						asmout << "\tlea eax, " << lex.idTable.table[lex.lexTable.table[i - 2].idxTI].id << '\n';
						asmout << "\tpush eax\n";
					}
					else {
						asmout << "\tpush OFFSET " << lex.idTable.table[lex.lexTable.table[i - 2].idxTI].id << '\n';
					}
					if (lex.idTable.table[lex.lexTable.table[i - 2].idxTI].iddatatype == IT::UINT) {
						asmout << "\tpush 1\n";
					}
					else if (lex.idTable.table[lex.lexTable.table[i - 2].idxTI].iddatatype == IT::DOUBLE) {
						asmout << "\tpush 2\n";
					}
					else if (lex.idTable.table[lex.lexTable.table[i - 2].idxTI].iddatatype == IT::BOOL) {
						asmout << "\tpush 3\n";
					}
					else if (lex.idTable.table[lex.lexTable.table[i - 2].idxTI].iddatatype == IT::STR) {
						asmout << "\tpush 4\n";
					}

					if (lex.idTable.table[lex.lexTable.table[i - 4].idxTI].idtype == IT::P && lex.idTable.table[lex.lexTable.table[i - 4].idxTI].iddatatype != IT::DOUBLE) {
						asmout << "\tlea eax, " << lex.idTable.table[lex.lexTable.table[i - 4].idxTI].id << '\n';
						asmout << "\tpush eax\n";
					}
					else {
						asmout << "\tpush OFFSET " << lex.idTable.table[lex.lexTable.table[i - 4].idxTI].id << '\n';
					}
					if (lex.idTable.table[lex.lexTable.table[i - 4].idxTI].iddatatype == IT::UINT) {
						asmout << "\tpush 1\n";
					}
					else if (lex.idTable.table[lex.lexTable.table[i - 4].idxTI].iddatatype == IT::DOUBLE) {
						asmout << "\tpush 2\n";
					}
					else if (lex.idTable.table[lex.lexTable.table[i - 4].idxTI].iddatatype == IT::BOOL) {
						asmout << "\tpush 3\n";
					}
					else if (lex.idTable.table[lex.lexTable.table[i - 4].idxTI].iddatatype == IT::STR) {
						asmout << "\tpush 4\n";
					}
					asmout << "\tcall compnum\n";
					asmout << "\tcmp eax, -1\n\tje THEENDER\n";
					asmout << "\tcmp eax, 0\n\tje notwhile" << whilei.top() << '\n';

	/*				asmout << "\tmov eax, " << lex.idTable.table[lex.lexTable.table[i - 4].idxTI].id << '\n';
					asmout << "\tcmp eax, " << lex.idTable.table[lex.lexTable.table[i - 2].idxTI].id << '\n';
					if (lex.lexTable.table[i - 3].lexema[0] == LEX_MORE)
					{
						asmout << "\tjle notwhile" << whilei.top() << '\n';
					}
					else if (lex.lexTable.table[i - 3].lexema[0] == LEX_LESS)
					{
						asmout << "\tjge notwhile" << whilei.top() << '\n';
					}
					else if (lex.lexTable.table[i - 3].lexema[0] == LEX_ISEQUAL)
					{
						asmout << "\tjnz notwhile" << whilei.top() << '\n';
					}
					else if (lex.lexTable.table[i - 3].lexema[0] == LEX_NOT_EQUAL)
					{
						asmout << "\tjz notwhile" << whilei.top() << '\n';
					}
					else if (lex.lexTable.table[i - 3].lexema[0] == LEX_MORE_OR_EQUAL)
					{
						asmout << "\tjl notwhile" << whilei.top() << '\n';
					}
					else if (lex.lexTable.table[i - 3].lexema[0] == LEX_LESS_OR_EQUAL)
					{
						asmout << "\tjg notwhile" << whilei.top() << '\n';
					}*/
				}
				break;
			case LEX_IF:
			{
				flag_if = true;
				string buf = IF + to_string(num_of_if);
				stk.push(buf);
				ifi.push(num_of_if);
				num_of_if++;
				break;
			}
			case LEX_WHILE:
			{
				flag_while = true;
				string buf = UNTILI + to_string(num_of_while);
				stk.push(buf);
				whilei.push(num_of_while);
				num_of_while++;
				break;
			}
			case LEX_ELSE:
			{
				flag_else = true;
				string buf = ELSEI + to_string(num_of_if - 1);
				stk.push(buf);
				break;
			}
			case LEX_LEFTTHESIS:
			{
				if (num_of_if && lex.lexTable.table[i - 1].lexema[0] == LEX_IF)
				{
					asmout << "\tmov al, \'" << lex.lexTable.table[i + 2].lexema[0] << "\'\n";
					asmout << "\tpush eax\n";
					if (lex.idTable.table[lex.lexTable.table[i + 3].idxTI].idtype == IT::P && lex.idTable.table[lex.lexTable.table[i + 3].idxTI].iddatatype != IT::DOUBLE) {
						asmout << "\tlea eax, " << lex.idTable.table[lex.lexTable.table[i + 3].idxTI].id << '\n';
						asmout << "\tpush eax\n";
					}
					else {
						asmout << "\tpush OFFSET " << lex.idTable.table[lex.lexTable.table[i + 3].idxTI].id << '\n';
					}
					if (lex.idTable.table[lex.lexTable.table[i + 3].idxTI].iddatatype == IT::UINT) {
						asmout << "\tpush 1\n";
					}
					else if (lex.idTable.table[lex.lexTable.table[i + 3].idxTI].iddatatype == IT::DOUBLE) {
						asmout << "\tpush 2\n";
					}
					else if (lex.idTable.table[lex.lexTable.table[i + 3].idxTI].iddatatype == IT::BOOL) {
						asmout << "\tpush 3\n";
					}
					else if (lex.idTable.table[lex.lexTable.table[i + 3].idxTI].iddatatype == IT::STR) {
						asmout << "\tpush 4\n";
					}

					if (lex.idTable.table[lex.lexTable.table[i + 1].idxTI].idtype == IT::P && lex.idTable.table[lex.lexTable.table[i + 1].idxTI].iddatatype != IT::DOUBLE) {
						asmout << "\tlea eax, " << lex.idTable.table[lex.lexTable.table[i + 1].idxTI].id << '\n';
						asmout << "\tpush eax\n";
					}
					else {
						asmout << "\tpush OFFSET " << lex.idTable.table[lex.lexTable.table[i + 1].idxTI].id << '\n';
					}
					if (lex.idTable.table[lex.lexTable.table[i + 1].idxTI].iddatatype == IT::UINT) {
						asmout << "\tpush 1\n";
					}
					else if (lex.idTable.table[lex.lexTable.table[i + 1].idxTI].iddatatype == IT::DOUBLE) {
						asmout << "\tpush 2\n";
					}
					else if (lex.idTable.table[lex.lexTable.table[i + 1].idxTI].iddatatype == IT::BOOL) {
						asmout << "\tpush 3\n";
					}
					else if (lex.idTable.table[lex.lexTable.table[i + 1].idxTI].iddatatype == IT::STR) {
						asmout << "\tpush 4\n";
					}
					asmout << "\tcall compnum\n";
					asmout << "\tcmp eax, -1\n\tje THEENDER\n";
					asmout << "\tcmp eax, 1\n\tje ifi" << ifi.top() << '\n';
					asmout << "\tcmp eax, 0\n\tje elsei" << ifi.top() << '\n';

				}

				break;
			}
			case LEX_PRINT:
			{
				switch (lex.idTable.table[lex.lexTable.table[i + 1].idxTI].iddatatype)
				{
				case IT::IDDATATYPE::UINT: {
					asmout << "\n\tINVOKE printss, 1, addr " << lex.idTable.table[lex.lexTable.table[i + 1].idxTI].id << "\n";
					break;
				}
				case IT::IDDATATYPE::BOOL: {
					asmout << "\n\tINVOKE printss, 3, addr " << lex.idTable.table[lex.lexTable.table[i + 1].idxTI].id << "\n";
					break;
				}
				case IT::IDDATATYPE::STR: {
					asmout << "\n\tINVOKE printss, 4, addr " << lex.idTable.table[lex.lexTable.table[i + 1].idxTI].id << "\n";
					break;
				}
				case IT::IDDATATYPE::DOUBLE: {
					asmout << "\n\tINVOKE printss, 2, addr " << lex.idTable.table[lex.lexTable.table[i + 1].idxTI].id << "\n";
					break;
				}
				}
				break;
			}
			case LEX_PRINTLN:
			{
				switch (lex.idTable.table[lex.lexTable.table[i + 1].idxTI].iddatatype)
				{
				case IT::IDDATATYPE::UINT: {
					asmout << "\n\tINVOKE printssln, 1, addr " << lex.idTable.table[lex.lexTable.table[i + 1].idxTI].id << "\n";
					break;
				}
				case IT::IDDATATYPE::BOOL: {
					asmout << "\n\tINVOKE printssln, 3, addr " << lex.idTable.table[lex.lexTable.table[i + 1].idxTI].id << "\n";
					break;
				}
				case IT::IDDATATYPE::STR: {
					asmout << "\n\tINVOKE printssln, 4, addr " << lex.idTable.table[lex.lexTable.table[i + 1].idxTI].id << "\n";
					break;
				}
				case IT::IDDATATYPE::DOUBLE: {
					asmout << "\n\tINVOKE printssln, 2, addr " << lex.idTable.table[lex.lexTable.table[i + 1].idxTI].id << "\n";
					break;
				}
				}
				break;
			}
			case LEX_READ:
			{
				switch (lex.idTable.table[lex.lexTable.table[i + 1].idxTI].iddatatype)
				{
				case IT::IDDATATYPE::UINT: {
					asmout << "\n\tINVOKE readss, 1, addr " << lex.idTable.table[lex.lexTable.table[i + 1].idxTI].id << "\n";
					break;
				}
				case IT::IDDATATYPE::BOOL: {
					asmout << "\n\tINVOKE readss, 3, addr " << lex.idTable.table[lex.lexTable.table[i + 1].idxTI].id << "\n";
					break;
				}
				case IT::IDDATATYPE::STR: {
					asmout << "\n\tINVOKE readss, 4, addr " << lex.idTable.table[lex.lexTable.table[i + 1].idxTI].id << "\n";
					break;
				}
				case IT::IDDATATYPE::DOUBLE: {
					asmout << "\n\tINVOKE readss, 2, addr " << lex.idTable.table[lex.lexTable.table[i + 1].idxTI].id << "\n";
					break;
				}
				}
					asmout << "\tcmp eax, -1\n\tjz THEENDER\n"; 

				break;
			}
			}
		}
		asmout.close();
	}
}