#include "Out.h"
#include "stdafx.h"
#include <cstdlib>
#pragma warning(disable:4996)

namespace Out {
	OUT getout(wchar_t outfile[]) { // создание потока вывода
		OUT out;
		out.stream = new ofstream;
		char narrow_outfile[300];
		wcstombs(narrow_outfile, outfile, 300);
		out.stream->open(narrow_outfile);
		if (!out.stream->is_open()) {
			throw ERROR_THROW(13);
		}
		std::wcscpy(out.outfile, outfile);
		return out;
	}
	void WriteOut(In::IN in, wchar_t outfile[]) { // запись выходного файла
		char narrow_outfile[300];
		wcstombs(narrow_outfile, outfile, 300);
		ofstream fout(narrow_outfile);
		fout << in.text;
		fout.close();
	}
	void WriteError(OUT out, Error::ERROR er) { // вывод ошибки
		if (out.stream) {
			*out.stream << "Ошибка " << er.id << ": " << er.message;
			if (er.inext.line != -1) {
				*out.stream << ", строка " << er.inext.line << ", позиция " << er.inext.col << '\n';
			}
			else {
				*out.stream << '\n';
			}
		}
		else {
			cout << "Ошибка " << er.id << ": " << er.message << '\n';
			cout << "Строка " << er.inext.line << ", позиция " << er.inext.col << '\n';
		}
	}
	void Close(OUT out) { // закрытие
		out.stream->close();
		delete out.stream;
	}
}