#include "../Headers/Log.h"
#include "../stdafx.h"
#include <time.h>
#pragma warning(disable:4996)

namespace Log {
	LOG getlog(wchar_t logfile[]) { // создание и открытие потокового вывода протокола
		LOG log;
		log.stream = new ofstream; // создаем новый поток для записи

		int filenameLen = wcslen(logfile) + 1;
		char filename[filenameLen];
		wcstombs(filename, logfile, filenameLen);

		log.stream->open(filename); // открываем поток по переданному названию файла
		if (!log.stream->is_open()) { // если не открылся
			throw ERROR_THROW(112); // вызываем исключение
		}
		std::wcscpy(log.logfile, logfile); // копируем название файла
		return log;
	}
	void WriteLine(LOG log, char* c, ...) { // используется для вывода строки в протокол
		char** p = &c; // создаем указатель для считвание неизвестного числа строк
		int j = 0; // счетчик
		while (p[j] != "") { // пока не встретилась пустая строка
			*log.stream << p[j++]; // записываем текушую в log и переходим на следующую 
		}
	}
	void WriteLine(LOG log, wchar_t* c, ...) { // тоже, что и предыдущая функция, но, если параметры wchar_t*
		wchar_t** p = &c;
		char buf[50];
		int j = 0;
		while (p[j] != L"") {
			wcstombs(buf, p[j++], 50); // преобразование wchar_t* в char*
			*log.stream << buf;
		}
	}
	void WriteLog(LOG log) { // функция для вывода заголовка в log
		char buf[50]; // буферная переменная
		time_t curtime; // переменная для времени
		curtime = time(NULL); // текущее время
		tm localtime; // структура для времени
		localtime_r(&curtime, &localtime); // время в time_t записывает в структуру tm
		strftime(buf, 50, "%Y.%m.%d %H:%M:%S", &localtime); // преобразуем дату и время в строку по заданному формату
		*log.stream << "---- Протокол ---- " << buf << " ----------" << '\n';
	}
	void WriteParm(LOG log, Parm::PARm parm) { // функция для вывода информации о выходных параметрах
		*log.stream << "---- Параметры ----" << '\n';
		char buf[PARM_MAX_SIZE]; // буферная переменная
		wcstombs(buf, parm.log, PARM_MAX_SIZE);  // преобразование wchar_t* в char*
		*log.stream << "-log: " << buf << '\n'; // записываем про log
		wcstombs(buf, parm.out, PARM_MAX_SIZE); // преобразование wchar_t* в char*
		*log.stream << "-out: " << buf << '\n'; // записываем про out
		wcstombs(buf, parm.in, PARM_MAX_SIZE); // преобразование wchar_t* в char*
		*log.stream << "-in: " << buf << '\n'; // записываем про in
	}
	void WriteIn(LOG log, In::iN in) { // вывод информации о тексте в файле
		*log.stream << "---- Исходные данные ----" << '\n';
		*log.stream << "Количество символов: " << in.size << '\n';
		*log.stream << "Проигнорировано    :" << in.ignore << '\n';
		*log.stream << "Количество строк   :" << in.lines << '\n';
	}
	void WriteError(LOG log, Error::ERROr er) { // вывод информации об ошибке
		if (log.stream) { // если открыт поток для записи
			*log.stream << "Ошибка " << er.id << ": " << er.message; // выводим сообщение об ошибке
			if (er.inext.line != -1) { // если имеется информации о месте ошибке, то выводим и эту информацию
				*log.stream << ", строка " << er.inext.line << ", позиция " << er.inext.col << '\n';
			}
			else {
				*log.stream << '\n';
			}
		}
		else { // если поток не открыт, то выводим информацию в консоль
			cout << "Ошибка " << er.id << ": " << er.message << '\n';
			cout << "Строка " << er.inext.line << ", позиция " << er.inext.col << '\n';
		}
	}
	void Close(LOG log) { // закрываем поток
		log.stream->close();
		delete log.stream;
	}
}