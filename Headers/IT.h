#pragma once
#include <map>
#include <cstring>
#define ID_MAXSIZE 16 // максимальное количество символов в идентификаторе
#define TI_MAXSIZE 4096 // максимальное количество строк в таблице
#define TI_INT_DEFAULT 0x00000000 // значение по умолчанию для integer
#define TI_STR_DEFAULT 0x00 // значение по умолчанию для string
#define TI_NULLIDX 0xffffffff // нет элемента в таблице идентификаторов
#define TI_STR_MAXSIZE 255 

namespace IT { // таблица идентификаторов
	enum IDDATATYPE { UINT = 1, STR = 2, BOOL = 3, DOUBLE = 4}; // тип данных идентификаторов
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4 }; // тип идентификаторов: переменная, функция, параметр, литерал
	struct Entry
	{
		int			idxfirstLE; // индекс первой строки в таблице лексем
		char		id[17];
		IDDATATYPE	iddatatype;
		IDTYPE		idtype;
		string scope_name;
		union
		{
			double vint = 0.0;
			struct
			{
				int len;
				char str[TI_STR_MAXSIZE - 1];
			}	vstr[TI_STR_MAXSIZE];
		} value;
		Entry(int IdxfirstLE, const char* Id, IDDATATYPE Datatype, IDTYPE Type, string Scope, int value)
		{
			this->idxfirstLE = IdxfirstLE;
			for (int i = 0; i < strlen(Id); i++)
			{
				this->id[i] = Id[i];
			}
			this->id[strlen(Id)] = '\0';
			this->iddatatype = Datatype;
			this->idtype = Type;
			this->scope_name = Scope;
			this->value.vint = value;
		}
		Entry() = default;
	};

	struct IdTable { // экземпляр таблицы идентификаторов
		int maxsize; // емкость таблиц идентификаторов < TI_MAXSIZE
		int size; // текущий размер таблицы идентификаторов < maxsize
		Entry* table; // массив строк в таблице идентификаторов
	};
	IdTable Create(int size); // создать таблицу идентификаторов
	void Add(IdTable& idtable, Entry entry); // добавить строку в таблицу идентификаторов 
	int search(IdTable& idtable, IT::Entry entry, map<string, string> scop);
	int search(IdTable& idtable, IT::Entry entry);
	Entry GetEntry(IdTable& idtable, int n); // получить строку из таблицы идентификаторов
	void Delete(IdTable& idtable); // удалить таблицу идентификаторов
}