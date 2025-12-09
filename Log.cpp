#include "Log.h"

#include <cstring>

#include "stdafx.h"
#include <time.h>
#pragma warning(disable:4996)

namespace Log {
	LOG getlog(wchar_t logfile[]) { // создание протокола
		LOG log;
		log.stream = new ofstream;
		char narrow_logfile[300];
		wcstombs(narrow_logfile, logfile, 300);
		log.stream->open(narrow_logfile);
		if (!log.stream->is_open()) {
			throw ERROR_THROW(12);
		}
		std::wcscpy(log.logfile, logfile);
		return log;
	}
	void WriteLine(LOG log, char* c, ...) { // вывод строки
		char** p = &c;
		int j = 0;
		while (p[j] != "") {
			*log.stream << p[j++];
		}
	}
	void WriteLine(LOG log, wchar_t* c, ...) { // вывод строки (wchar_t)
		wchar_t** p = &c;
		char buf[50];
		int j = 0;
		while (p[j] != L"") {
			wcstombs(buf, p[j++], 50);
			*log.stream << buf;
		}
	}
	void WriteLog(LOG log) { // заголовок протокола
		char buf[50];
		time_t curtime;
		curtime = time(NULL);
		tm local_tm_struct;
		tm* ltime_ptr = std::localtime(&curtime);
		if (ltime_ptr) {
		    local_tm_struct = *ltime_ptr;
		} else {
			std::memset(&local_tm_struct, 0, sizeof(tm));
		}
		strftime(buf, 50, "%Y.%m.%d %H:%M:%S", &local_tm_struct);
		*log.stream << "---- Протокол ---- " << buf << " ----------" << '\n';
	}
	void WriteParm(LOG log, Parm::Parm parm) { // вывод параметров
		*log.stream << "---- Параметры ----" << '\n';
		char buf[PARM_MAX_SIZE];
		wcstombs(buf, parm.log, PARM_MAX_SIZE);
		*log.stream << "-log: " << buf << '\n';
		wcstombs(buf, parm.out, PARM_MAX_SIZE);
		*log.stream << "-out: " << buf << '\n';
		wcstombs(buf, parm.in, PARM_MAX_SIZE);
		*log.stream << "-in: " << buf << '\n';
	}
	void WriteIn(LOG log, In::IN in) { // статистика входных данных
		*log.stream << "---- Исходные данные ----" << '\n';
		*log.stream << "Количество символов: " << in.size << '\n';
		*log.stream << "Проигнорировано    :" << in.ignore << '\n';
		*log.stream << "Количество строк   :" << in.lines << '\n';
	}
	void WriteError(LOG log, Error::ERROR er) { // вывод ошибки
		if (log.stream) {
			*log.stream << "Ошибка " << er.id << ": " << er.message;
			if (er.inext.line != -1) {
				*log.stream << ", строка " << er.inext.line << ", позиция " << er.inext.col << '\n';
			}
			else {
				*log.stream << '\n';
			}
		}
		else {
			cout << "Ошибка " << er.id << ": " << er.message << '\n';
			cout << "Строка " << er.inext.line << ", позиция " << er.inext.col << '\n';
		}
	}
	void Close(LOG log) { // закрытие
		log.stream->close();
		delete log.stream;
	}
}