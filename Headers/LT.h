#pragma once
#define LEXEMA_FIXSIZE 1 // фиксированный размер лексемы
#define LT_MAXSIZE 4096 // максимальное число строк в таблице лексем
#define LT_TI_NULLIDX 0xffffffff // нет элемента таблицы идентификаторов

// Типы данных (ключевые слова LSA-2025)
#define LEX_UINT_TYPE 'U' // лексема для unsigned integer
#define LEX_CHAR_TYPE 'H' // лексема для char
#define LEX_LOGIC_TYPE 'G' // лексема для logic

// Ключевые слова LSA-2025
#define LEX_UNSIGNED 'u'
#define LEX_INTEGER 'n'
#define LEX_ID 'i' // лексема для идентификатора
#define LEX_MAIN 'm' // лексема для main
#define LEX_LITERAL 'l' // лексема для литерала
#define LEX_FUNC 'f' // лексема для func
#define LEX_SEND 's' // лексема для send
#define LEX_IF 'Y' // лексема для if
#define LEX_DIFFER 'D' // лексема для differ
#define LEX_BECAUSE 'B' // лексема для because

// Разделители
#define LEX_SEMICOLON ';' // лексема для ;
#define LEX_COMMA ',' // лексема для ,
#define LEX_LEFTBRACE '{' // лексема для {
#define LEX_BRACELET '}' // лексема для }
#define LEX_LEFTTHESIS '(' // лексема для (
#define LEX_RIGHTTHESIS ')' // лексема для )

// Операторы
#define LEX_PLUS '+' // лексема для +
#define LEX_MINUS '-' // лексема для -
#define LEX_STAR '*' // лексема для *
#define LEX_COLON ':' // лексема для : (деление)
#define LEX_EQUAL '=' // лексема для = (присваивание)
#define LEX_ISEQUAL 'E' // лексема для == (сравнение на равенство)
#define LEX_MORE '>' // лексема для >
#define LEX_LESS '<' // лексема для <
#define LEX_BITNOT '~' // лексема для ~ (побитовое НЕ)
#define LEX_NOT_EQUAL 'N' // лексема для !=
#define LEX_MORE_OR_EQUAL 'O' // лексема для >=
#define LEX_LESS_OR_EQUAL 'Q' // лексема для <=
#define LEX_INCREMENT 'P' // лексема для ++
#define LEX_DECREMENT 'T' // лексема для --

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