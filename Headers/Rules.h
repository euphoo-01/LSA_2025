#pragma once
#include "GRB.h"
#include "Error.h"
#include "LT.h"
#include "LexA.h"
#define NS(n) GRB::Rule::Chain::N(n)
#define TS(n) GRB::Rule::Chain::T(n)
#define ISNS(n)	GRB::Rule::Chain::isN(n)
#define GRB_ERROR_SERIES 600

namespace GRB {
	Greibach greibach(NS('S'), TS('$'),
		27, // Общее количество правил
		// S: Программа -> СписокОбъявленийФункций ГлавнаяФункция | ГлавнаяФункция
		Rule(NS('S'), GRB_ERROR_SERIES + 0,
			2,
			Rule::Chain(2, NS('F'), NS('X')), // СписокОбъявленийФункций ГлавнаяФункция
			Rule::Chain(1, NS('X'))          // Только ГлавнаяФункция
		),

		// F: СписокОбъявленийФункций -> ЕдиничноеОбъявлениеФункции СписокОбъявленийФункций | пусто
		Rule(NS('F'), GRB_ERROR_SERIES + 1,
			2,
			Rule::Chain(2, NS('G'), NS('F')),
			Rule::Chain(0) // пусто
		),

		// G: ЕдиничноеОбъявлениеФункции -> func <TYPE> <ID> ( <PARAMS> ) { <BLOCK_STATEMENTS> send <EXPRESSION> ; }
		Rule(NS('G'), GRB_ERROR_SERIES + 2,
			1,
			Rule::Chain(13, TS(LEX_FUNC), NS('T'), TS(LEX_ID), TS(LEX_LEFTTHESIS), NS('P'), TS(LEX_RIGHTTHESIS), TS(LEX_LEFTBRACE), NS('B'), TS(LEX_SEND), NS('E'), TS(LEX_SEMICOLON), TS(LEX_BRACELET), TS(LEX_SEMICOLON))
		),

		// X: ГлавнаяФункция -> main { <BLOCK_STATEMENTS> send <EXPRESSION> ; }
		Rule(NS('X'), GRB_ERROR_SERIES + 3,
			1,
			Rule::Chain(8, TS(LEX_MAIN), TS(LEX_LEFTBRACE), NS('B'), TS(LEX_SEND), NS('E'), TS(LEX_SEMICOLON), TS(LEX_BRACELET), TS(LEX_SEMICOLON))
		),

		// T: Тип -> unsigned integer | char | logic (Это теперь одиночные лексемы)
		Rule(NS('T'), GRB_ERROR_SERIES + 4,
			3,
			Rule::Chain(1, TS(LEX_UINT_TYPE)),
			Rule::Chain(1, TS(LEX_CHAR_TYPE)),
			Rule::Chain(1, TS(LEX_LOGIC_TYPE))
		),

		// P: СписокПараметров -> Тип ID БольшеПараметров | пусто
		Rule(NS('P'), GRB_ERROR_SERIES + 5,
			2,
			Rule::Chain(3, NS('T'), TS(LEX_ID), NS('Y')),
			Rule::Chain(0) // пусто
		),

		// Y: БольшеПараметров -> , Тип ID БольшеПараметров | пусто
		Rule(NS('Y'), GRB_ERROR_SERIES + 6,
			2,
			Rule::Chain(4, TS(LEX_COMMA), NS('T'), TS(LEX_ID), NS('Y')),
			Rule::Chain(0) // пусто
		),

		// B: БлокОператоров -> Оператор ; B | пусто
		Rule(NS('B'), GRB_ERROR_SERIES + 7,
			2,
			Rule::Chain(3, NS('D'), TS(LEX_SEMICOLON), NS('B')), // D: ЕдиничныйОператор
			Rule::Chain(0) // пусто
		),

		// D: Оператор -> ОбъявлениеПеременной | Присваивание | ВызовФункции | УсловныйОператорIf | ЦиклBecause
		Rule(NS('D'), GRB_ERROR_SERIES + 8,
			5,
			Rule::Chain(1, NS('V')), // ОбъявлениеПеременной
			Rule::Chain(1, NS('A')), // Присваивание
			Rule::Chain(1, NS('C')), // ВызовФункции (используется для вызовов функций, которые являются операторами)
			Rule::Chain(1, NS('I')), // УсловныйОператорIf
			Rule::Chain(1, NS('J'))  // ЦиклBecause
		),

		// V: ОбъявлениеПеременной -> Тип ID
		Rule(NS('V'), GRB_ERROR_SERIES + 9,
			1,
			Rule::Chain(2, NS('T'), TS(LEX_ID))
		),

		// A: Присваивание -> ID = Выражение
		Rule(NS('A'), GRB_ERROR_SERIES + 10,
			1,
			Rule::Chain(3, TS(LEX_ID), TS(LEX_EQUAL), NS('E'))
		),

		// C: ВызовФункции -> ID ( СписокАргументов )
		Rule(NS('C'), GRB_ERROR_SERIES + 11,
			1,
			Rule::Chain(4, TS(LEX_ID), TS(LEX_LEFTTHESIS), NS('R'), TS(LEX_RIGHTTHESIS))
		),

		// R: СписокАргументов -> Выражение БольшеАргументов | пусто
		Rule(NS('R'), GRB_ERROR_SERIES + 12,
			2,
			Rule::Chain(2, NS('E'), NS('Q')),
			Rule::Chain(0) // пусто
		),

		// Q: БольшеАргументов -> , Выражение БольшеАргументов | пусто
		Rule(NS('Q'), GRB_ERROR_SERIES + 13,
			2,
			Rule::Chain(3, TS(LEX_COMMA), NS('E'), NS('Q')),
			Rule::Chain(0) // пусто
		),

		// I: УсловныйОператорIf -> if ( ЛогическоеВыражение ) { БлокОператоров } ЧастьDiffer
		Rule(NS('I'), GRB_ERROR_SERIES + 14,
			1,
			Rule::Chain(9, TS(LEX_IF), TS(LEX_LEFTTHESIS), NS('L'), TS(LEX_RIGHTTHESIS), TS(LEX_LEFTBRACE), NS('B'), TS(LEX_BRACELET), NS('_'))
		),

		// _: ЧастьDiffer -> differ { БлокОператоров } | пусто
		Rule(NS('_'), GRB_ERROR_SERIES + 15,
			2,
			Rule::Chain(4, TS(LEX_DIFFER), TS(LEX_LEFTBRACE), NS('B'), TS(LEX_BRACELET)),
			Rule::Chain(0) // пусто
		),

		// L: ЛогическоеВыражение -> Выражение ОператорСравнения Выражение
		Rule(NS('L'), GRB_ERROR_SERIES + 16,
			1,
			Rule::Chain(3, NS('E'), NS('U'), NS('E'))
		),

		// U: ОператорСравнения -> == | != | < | > | <= | >=
		Rule(NS('U'), GRB_ERROR_SERIES + 17,
			6,
			Rule::Chain(1, TS(LEX_ISEQUAL)),
			Rule::Chain(1, TS(LEX_NOT_EQUAL)),
			Rule::Chain(1, TS(LEX_LESS)),
			Rule::Chain(1, TS(LEX_MORE)),
			Rule::Chain(1, TS(LEX_LESS_OR_EQUAL)),
			Rule::Chain(1, TS(LEX_MORE_OR_EQUAL))
		),

		// J: ЦиклBecause -> because ( ИнициализацияДляBecause ; ЛогическоеВыражение ; ОперацияДляBecause ) { БлокОператоров }
		Rule(NS('J'), GRB_ERROR_SERIES + 18,
			1,
			Rule::Chain(11, TS(LEX_BECAUSE), TS(LEX_LEFTTHESIS), NS('K'), TS(LEX_SEMICOLON), NS('L'), TS(LEX_SEMICOLON), NS('Z'), TS(LEX_RIGHTTHESIS), TS(LEX_LEFTBRACE), NS('B'), TS(LEX_BRACELET))
		),

		// K: ИнициализацияДляBecause -> Тип ID = Выражение | ID = Выражение
		Rule(NS('K'), GRB_ERROR_SERIES + 19,
			2,
			Rule::Chain(4, NS('T'), TS(LEX_ID), TS(LEX_EQUAL), NS('E')),
			Rule::Chain(3, TS(LEX_ID), TS(LEX_EQUAL), NS('E'))
		),

		// Z: ОперацияДляBecause -> ID УнарныйОператор (Inc/Dec) | ID = Выражение | пусто
		Rule(NS('Z'), GRB_ERROR_SERIES + 20,
			3,
			Rule::Chain(2, TS(LEX_ID), NS('H')), // ID УнарныйОператор (Inc/Dec)
			Rule::Chain(3, TS(LEX_ID), TS(LEX_EQUAL), NS('E')), // ID = Выражение
			Rule::Chain(0) // допустима пустая операция
		),

		// E: Выражение -> Терм ОператорСложения
		Rule(NS('E'), GRB_ERROR_SERIES + 21,
			1,
			Rule::Chain(2, NS('V'), NS('AD')) // V: Терм, AD: ОператорСложения
		),

		// V: Терм -> Фактор ОператорУмножения
		Rule(NS('V'), GRB_ERROR_SERIES + 22,
			1,
			Rule::Chain(2, NS('W'), NS('ML')) // W: Фактор, ML: ОператорУмножения
		),

		// AD: ОператорСложения -> + Терм ОператорСложения | - Терм ОператорСложения | пусто
		Rule(NS('AD'), GRB_ERROR_SERIES + 23,
			3,
			Rule::Chain(3, TS(LEX_PLUS), NS('V'), NS('AD')),
			Rule::Chain(3, TS(LEX_MINUS), NS('V'), NS('AD')),
			Rule::Chain(0) // пусто
		),

		// ML: ОператорУмножения -> * Фактор ОператорУмножения | : Фактор ОператорУмножения | пусто
		Rule(NS('ML'), GRB_ERROR_SERIES + 24,
			3,
			Rule::Chain(3, TS(LEX_STAR), NS('W'), NS('ML')),
			Rule::Chain(3, TS(LEX_COLON), NS('W'), NS('ML')),
			Rule::Chain(0) // пусто
		),

		// W: Фактор -> LITERAL | ID | ( Выражение ) | УнарныйОператор Фактор | ВызовФункции
		Rule(NS('W'), GRB_ERROR_SERIES + 25,
			5,
			Rule::Chain(1, TS(LEX_LITERAL)),
			Rule::Chain(1, TS(LEX_ID)),
			Rule::Chain(3, TS(LEX_LEFTTHESIS), NS('E'), TS(LEX_RIGHTTHESIS)),
			Rule::Chain(2, NS('H'), NS('W')), // H: УнарныйОператор
			Rule::Chain(1, NS('C')) // C: ВызовФункции (уже определен)
		),

		// H: УнарныйОператор -> ~ | ++ | --
		Rule(NS('H'), GRB_ERROR_SERIES + 26,
			3,
			Rule::Chain(1, TS(LEX_BITNOT)),
			Rule::Chain(1, TS(LEX_INCREMENT)),
			Rule::Chain(1, TS(LEX_DECREMENT))
		)
	);
}