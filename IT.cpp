#include <iostream>
#include "stdafx.h"
#include "IT.h"

#include <cstring>

namespace IT {
	// создание
	IdTable Create(int size) {
		IdTable* idtable = new IdTable;
		if (size > TI_MAXSIZE) {
			throw ERROR_THROW(17);
		}
		idtable->maxsize = size;
		idtable->size = 0;
		idtable->table = new Entry[size];
		return *idtable;
	}

	// добавление
	void Add(IdTable& idtable, Entry entry) {
		if (idtable.size + 1 > idtable.maxsize) {
			throw ERROR_THROW(18);
		}
		idtable.table[idtable.size] = entry;
		idtable.size++;
	}

	// получить элемент
	Entry GetEntry(IdTable& idtable, int n) {
		if (n < 0 || n > idtable.size - 1) {
			throw ERROR_THROW(19);
		}
		return idtable.table[n];
	}
	
	// поиск
	int search(IdTable& idtable, IT::Entry entry, map<string, string> scope)
	{
		for (int j = 0; j < idtable.size; j++)
		{
			if (std::strcmp(entry.id, idtable.table[j].id) == 0 && entry.scope_name == idtable.table[j].scope_name)
			{
				return j;
			}
		}
		string buf = scope[entry.scope_name];
		while (buf != "") {
			for (int j = 0; j < idtable.size; j++)
			{
				if (std::strcmp(entry.id, idtable.table[j].id) == 0 && idtable.table[j].scope_name == buf)
				{
					return j;
				}
			}
			buf = scope[buf];
		}
		return -1;
	}
	
	// поиск в текущей области видимости
	int search(IdTable& idtable, IT::Entry entry)
	{
		for (int j = 0; j < idtable.size; j++)
		{
			if (std::strcmp(entry.id, idtable.table[j].id) == 0 && entry.scope_name == idtable.table[j].scope_name)
			{
				return j;
			}
		}
		return -1;
	}

	// удаление
	void Delete(IdTable& idtable) {
		delete idtable.table;
		idtable.maxsize = 0;
		idtable.size = 0;
	}
}
