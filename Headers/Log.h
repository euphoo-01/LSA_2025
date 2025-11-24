#pragma once
#include <fstream>
#include "In.h"
#include "Parm.h"
#include "Error.h"
using namespace std;

namespace Log { // работа с протоколом
    struct LOG { // протокол
        wchar_t logfile[PARM_MAX_SIZE]; // имя файла протокола
        ofstream* stream; // выходной поток
    };

    static const LOG INITLOG{ L"", NULL };
    LOG getlog(wchar_t logfile[]); // для создания и открытия потокового вывода протокола
    void WriteLine(LOG log, char* c, ...); // запись строки в протокол
    void WriteLine(LOG log, wchar_t* c, ...); // запись строки в протокол
    void WriteLog(LOG log); // запись заголовка в протокол
    void WriteParm(LOG log, Parm::PARm parm); // запись информации о входных параметрах
    void WriteIn(LOG log, In::iN in); // запись о содержании файла
    void WriteError(LOG log, Error::ERROr error); // вывод информации об ошибке
    void Close(LOG log); // закрытие потока для записи
}