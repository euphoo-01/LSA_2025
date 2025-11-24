#pragma once
#include <fstream>
#include "In.h"
#include "Parm.h"
#include "Error.h"

using namespace std;

namespace Out {
    struct OUt {
        wchar_t outfile[PARM_MAX_SIZE]; // имя выходного файла
        ofstream* stream; // выходной поток
    };

    static const OUt INITOUT{ L"", NULL };
    OUt getout(wchar_t outfile[]); // для создания и открытия потокового вывода в выходной файл
    void WriteOut(In::iN, wchar_t outfile[]); // запись заголовка в протокол
    void WriteError(OUt out, Error::ERROr error); // вывод информации об ошибке
    void Close(OUt out); // закрытие потока для записи
}