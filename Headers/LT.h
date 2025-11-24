#pragma once
#define LEXEMA_FIXSIZE 1 // фиксированный размер лексемы
#define LT_MAXSIZE 4096 // максимальное число строк в таблице лексем
#define LT_TI_NULLIDX 0xffffffff // нет элемента таблицы идентификаторов
#define LEX_UINTEGER 't' // лексема для integer
#define LEX_STRING 't' // лексема для string
#define LEX_BOOL 't'
#define LEX_DOUBLE 't'
#define LEX_ID 'i' // лексема для идентификатора
#define LEX_MAIN 'm' // лексема для main
#define LEX_LITERAL 'l' // лексема для литерала
#define LEX_FUNCTION 'f' // лексема для function
#define LEX_RETURN 'r' // лексема для return
#define LEX_PRINT 'p' // лексема для print
#define LEX_PRINTLN 'x'
#define LEX_READ 'h'
#define LEX_SEMICOLON ';' // лексема для ;
#define LEX_COMMA ',' // лексема для ,
#define LEX_LEFTBRACE '{' // лексема для {
#define LEX_BRACELET '}' // лексема для }
#define LEX_LEFTTHESIS '(' // лексема для (
#define LEX_RIGHTTHESIS ')' // лексема для )
#define LEX_PLUS '+' // лексема для +
#define LEX_MINUS '-' // лексема для -
#define LEX_STAR '*' // лексема для *
#define LEX_DIRSLASH '/' // лексема для /
#define LEX_EQUAL '=' // лексема для =
#define LEX_ISEQUAL 'c' // лексема для ==
#define LEX_MORE '>'
#define LEX_LESS '<'
#define LEX_NOT '!'
#define LEX_OST '%'
#define LEX_NOT_EQUAL 'n'
#define LEX_MORE_OR_EQUAL 'e'
#define LEX_LESS_OR_EQUAL 'o'
#define LEX_IF 'y' // лексема для if
#define LEX_ELSE 'd'
#define LEX_WHILE 'w' // лексема для while
#define LEX_UNTIL 'u' // лексема для until
#define LEX_OTHERWISE 'o' // лексема для otherwise
#define LEX_LEN 'a'
#define LEX_COMP 'b'
#define LEX_AND '&'
#define LEX_OR '|'

namespace LT { // таблица лексем
	struct Entry { // строка таблицы лексем
		char lexema[LEXEMA_FIXSIZE]; // лексема
		int sn; // номер строки в исходном тексте
		int idxTI; // индекс в таблице идентификаторов или LT_TI_NULLIDX
	};

	struct LexTable { // экземпляр таблицы лексем
		int maxsize = 0; // емкость таблицы лексем < LT_MAXSIZE
		int size = 0; // текущий размер таблицы лексем < maxsize
		Entry* table; // массив строк таблицы лексем
	};

	LexTable Create(int size); // создать таблицу
	void Add(LexTable& lextable, Entry entry); // добавить строку в таблицу лексем
	Entry GetEntry(LexTable& lextable, int n); // получить строку из таблицы лексем
	void Delete(LexTable& lextable); // удалить таблицу лексем
	void WriteInFile(LexTable& lextable);
}