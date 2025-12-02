#pragma once
#include <fstream>
#include "In.h"
#include "Parm.h"
#include "Error.h"

using namespace std;

namespace Out {
	struct OUT {
		wchar_t outfile[PARM_MAX_SIZE]; // имя выходного файла
		ofstream* stream; // выходной поток
	};

	static const OUT INITOUT{ L"", NULL };
	OUT getout(wchar_t outfile[]); // для создания и открытия потокового вывода в выходной файл
	void WriteOut(In::IN, wchar_t outfile[]); // запись заголовка в протокол
	void WriteError(OUT out, Error::ERROR error); // вывод информации об ошибке
	void Close(OUT out); // закрытие потока для записи
}