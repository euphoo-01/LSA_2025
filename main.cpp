#include <iostream>
#include "stdafx.h"
#include <locale>
#include <cwchar>
#include "Headers/asmGenerator.h"
using namespace std;
int main(int argc, char* argv[]) {
    wchar_t* wargv[argc];
    for (int i = 0; i < argc; ++i) {
        size_t len = strlen(argv[i]) + 1; // +1 для \0
        wargv[i] = new wchar_t[len];;
        mbstowcs(wargv[i], argv[i], len);
    }
    Parm::PARm parm;
    Log::LOG log;
    Out::OUt out;
    try
    {
        parm = Parm::getparm(argc, wargv);
        log = Log::INITLOG;
        out = Out::INITOUT;
        out = Out::getout(parm.out);
        log = Log::getlog(parm.log);
        In::iN in_result = In::getin(parm.in);
        Log::WriteLog(log);
        Log::WriteParm(log, parm);
        // Out::WriteOut(in_result, out.outfile);
        Log::WriteIn(log, in_result);
        LexA::LEX LeX = LexA::LexA(parm, in_result);
        ofstream st("ST.txt");
        MFST_TRACE_START(st);
        MFST::Mfst mfst(LeX.lexTable, GRB::getGreibach());
        mfst.start(st);

        mfst.savededucation();

        mfst.printrules(st);
        // SemA::checkSemantic(LeX.lexTable, LeX.idTable, LeX.functions);
        // RPN::searchNextPosForCheck(LeX.lexTable, LeX.idTable);
        LT::WriteInFile(LeX.lexTable);
        // ASMGenerator::asmGenerator(LeX, parm.out);
        Log::Close(log);
        Out::Close(out);
    }
    catch (Error::ERROr e)
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