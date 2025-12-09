#include <iostream>
#include "stdafx.h"
#include <locale>
#include <cwchar>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <exception>

#include "Error.h"
#include "Parm.h"
#include "Log.h"
#include "Out.h"
#include "In.h"
#include "Lex.h"
#include "MFST.h"
#include "GRB.h"
#include "Sem.h"
#include "RPN.h"
#include "LT.h"
#include "NasmGen.h"

using namespace std;

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "ru_RU.UTF-8");

	std::vector<wchar_t*> argv_w(argc);
	std::vector<std::vector<wchar_t>> argv_w_data(argc);
	for (int i = 0; i < argc; ++i) {
		size_t len = mbstowcs(nullptr, argv[i], 0);
		if (len == (size_t)-1) {
			len = 0; 
		}
		argv_w_data[i].resize(len + 1);
		mbstowcs(argv_w_data[i].data(), argv[i], len + 1);
		argv_w[i] = argv_w_data[i].data();
	}

	Parm::Parm parm;
	Log::LOG log;
	Out::OUT out;
	try
	{
		parm = Parm::getParm(argc, argv_w.data());
		log = Log::INITLOG;
		out = Out::INITOUT;
		out = Out::getout(parm.out);
		log = Log::getlog(parm.log);
		In::IN in_result = In::getin(parm.in);
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		//Out::WriteOut(in_result, out.outfile);
		Log::WriteIn(log, in_result);
		Lex::LEX LexStruct = Lex::Lex(parm, in_result);
		ofstream st("ST.txt");
		MFST_TRACE_START(st);
		MFST::Mfst mfst(LexStruct.lexTable, GRB::getGreibach());
		mfst.start(st);

		mfst.savededucation();

		mfst.printrules(st);
		Sem::checkSemantic(LexStruct.lexTable, LexStruct.idTable, LexStruct.functions);
		RPN::searchAndConvert(LexStruct.lexTable, LexStruct.idTable);
		LT::WriteInFile(LexStruct.lexTable);
		NasmGen::asmGenerator(LexStruct, parm.out);
		Log::Close(log);
		Out::Close(out);

		if (parm.build) {
			char nasm_cmd[1024];
			char link_cmd[1024];
			char out_asm[300];
			char out_obj[300];
			char out_exe[300];

			wcstombs(out_asm, parm.out, 300);
			
			// .asm -> .o
			std::string obj_file = out_asm;
			size_t lastdot = obj_file.find_last_of(".");
			if (lastdot == std::string::npos) obj_file += ".o";
			else obj_file = obj_file.substr(0, lastdot) + ".o";
			
			// .o -> exe (no ext)
			std::string exe_file = obj_file;
			lastdot = exe_file.find_last_of(".");
			if (lastdot != std::string::npos) exe_file = exe_file.substr(0, lastdot);

			sprintf(nasm_cmd, "nasm -f elf64 \"%s\" -o \"%s\"", out_asm, obj_file.c_str());
			sprintf(link_cmd, "g++ -o \"%s\" \"%s\" \"/home/euphoo/02. University/КП/LSA_2025 (ручная работа)/LSA_StdLib/libLSA_StdLib.a\" -no-pie", exe_file.c_str(), obj_file.c_str());

			cout << "Сборка..." << endl;
			cout << nasm_cmd << endl;
			if (std::system(nasm_cmd) == 0) {
				cout << link_cmd << endl;
				if (std::system(link_cmd) == 0) {
					cout << "Сборка выполнена успешно: " << exe_file << endl;
				} else {
					cout << "Ошибка линковки" << endl;
				}
			} else {
				cout << "Ошибка ассемблирования" << endl;
			}
		}
	}
	catch (Error::ERROR e)
	{
		cout << "Ошибка " << e.id << ':' << e.message << "\n\n";
		if (e.inext.line != -1 && e.inext.col != -1) {
			cout << "На строке " << e.inext.line << " символ " << e.inext.col << '\n';
		}
		if (e.inext.line > 0)
		{
			Log::WriteError(log, e);
			Log::Close(log);
			Out::Close(out);
		}
	}
	catch (const std::exception& e) {
		cout << "Системная ошибка: " << e.what() << endl;
		Log::WriteError(log, ERROR_THROW(1));
		Log::Close(log);
		Out::Close(out);
	}
	catch (...) {
		cout << "Неизвестная системная ошибка" << endl;
		Log::WriteError(log, ERROR_THROW(1));
		Log::Close(log);
		Out::Close(out);
	}

	return 0;
}