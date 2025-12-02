#include <iostream>
#include "stdafx.h"
#include <locale>
#include <cwchar>
#include <fstream>
#include <vector>
#include <cstdlib>

#include "Error.h"
#include "Parm.h"
#include "Log.h"
#include "Out.h"
#include "In.h"
#include "Lex.h"
// #include "MFST.h"
// #include "GRB.h"
// #include "SemA.h"
// #include "RPN.h"
#include "LT.h"
// #include "asmGenerator.h"

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
		Lex::LEX LeX = Lex::LexA(parm, in_result);
		// ofstream st("ST.txt");
		// MFST_TRACE_START(st);
		// MFST::Mfst mfst(LeX.lexTable, GRB::getGreibach());
		// mfst.start(st);

		// mfst.savededucation();

		// mfst.printrules(st);
		// SemA::checkSemantic(LeX.lexTable, LeX.idTable, LeX.functions);
		// RPN::searchNextPosForCheck(LeX.lexTable, LeX.idTable);
		// LT::WriteInFile(LeX.lexTable);
		// ASMGenerator::asmGenerator(LeX, parm.out);
		Log::Close(log);
		Out::Close(out);
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

	return 0;
}