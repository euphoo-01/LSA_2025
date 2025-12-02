#include <iostream>
#include "stdafx.h"
#include "LT.h"

namespace LT {

	LexTable Create(int size) {
		if (size > LT_MAXSIZE) {
			throw ERROR_THROW(114);
		}
		LexTable lextable;
		lextable.maxsize = size;
		lextable.size = 0;
		lextable.table = new Entry[size];
		return lextable;
	}

	void Add(LexTable& lextable, Entry entry) {
		if (lextable.size + 1 > lextable.maxsize) {
			throw ERROR_THROW(115);
		}
		lextable.table[lextable.size] = entry;
		lextable.size++;
	}

	Entry GetEntry(LexTable& lextable, int n) {
		if (n < 0 || n > lextable.size - 1) {
			throw ERROR_THROW(116);
		}
		return lextable.table[n];
	}

	void Delete(LexTable& lextable) {
		delete lextable.table;
		lextable.maxsize = 0;
		lextable.size = 0;
	}

	void WriteInFile(LexTable& lextable) {
		ofstream LT_file;
		LT_file.open("NewLT.txt");
		LT::Entry cur_lex;
		int currentLine = 1;
		LT_file << currentLine;
		LT_file << '\t';
		for (int i = 0; i < lextable.size; i++)
		{
			cur_lex = LT::GetEntry(lextable, i);
			if (currentLine != cur_lex.sn)
			{
				currentLine = cur_lex.sn;
				LT_file << '\n';
				LT_file << currentLine;
				LT_file << '\t';
			}
			LT_file << cur_lex.lexema[0];
		}
		LT_file.close();
	}
}