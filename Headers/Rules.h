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
		27, // Total number of rules
		// S: Program -> FuncDeclarationsList MainFunction | MainFunction
		Rule(NS('S'), GRB_ERROR_SERIES + 0,
			2,
			Rule::Chain(2, NS('F'), NS('X')), // FuncDeclarationsList MainFunction
			Rule::Chain(1, NS('X'))          // MainFunction only
		),

		// F: FuncDeclarationsList -> SingleFuncDeclaration FuncDeclarationsList | empty
		Rule(NS('F'), GRB_ERROR_SERIES + 1,
			2,
			Rule::Chain(2, NS('G'), NS('F')),
			Rule::Chain() // empty
		),

		        // G: ЕдиничноеОбъявлениеФункции -> func <TYPE> <ID> ( <PARAMS> ) { <BLOCK_STATEMENTS> send <EXPRESSION> ; }
				Rule(NS('G'), GRB_ERROR_SERIES + 2,
					1,
					Rule::Chain(12, TS(LEX_FUNC), NS('T'), TS(LEX_ID), TS(LEX_LEFTTHESIS), NS('P'), TS(LEX_RIGHTTHESIS), TS(LEX_LEFTBRACE), NS('B'), TS(LEX_SEND), NS('E'), TS(LEX_SEMICOLON), TS(LEX_BRACELET))
				),
		        // X: ГлавнаяФункция -> main { <BLOCK_STATEMENTS> send <EXPRESSION> ; }
				Rule(NS('X'), GRB_ERROR_SERIES + 3,
					1,
					Rule::Chain(7, TS(LEX_MAIN), TS(LEX_LEFTBRACE), NS('B'), TS(LEX_SEND), NS('E'), TS(LEX_SEMICOLON), TS(LEX_BRACELET))
				),
		        // T: Тип -> LEX_UNSIGNED LEX_INTEGER | LEX_CHAR_TYPE | LEX_LOGIC_TYPE
				Rule(NS('T'), GRB_ERROR_SERIES + 4,
					3,
					Rule::Chain(2, TS(LEX_UNSIGNED), TS(LEX_INTEGER)), // unsigned integer
					Rule::Chain(1, TS(LEX_CHAR_TYPE)),
					Rule::Chain(1, TS(LEX_LOGIC_TYPE))
				),
		// P: ParamsList -> Type ID MoreParams | empty
		Rule(NS('P'), GRB_ERROR_SERIES + 5,
			2,
			Rule::Chain(3, NS('T'), TS(LEX_ID), NS('Y')),
			Rule::Chain() // empty
		),

		// Y: MoreParams -> , Type ID MoreParams | empty
		Rule(NS('Y'), GRB_ERROR_SERIES + 6,
			2,
			Rule::Chain(4, TS(LEX_COMMA), NS('T'), TS(LEX_ID), NS('Y')),
			Rule::Chain() // empty
		),

		// B: BlockStatements -> Statement ; B | empty
		Rule(NS('B'), GRB_ERROR_SERIES + 7,
			2,
			Rule::Chain(3, NS('D'), TS(LEX_SEMICOLON), NS('B')), // D: Single Statement
			Rule::Chain() // empty
		),

		// D: Statement -> VariableDeclaration | Assignment | FunctionCall | IfStatement | BecauseLoop
		Rule(NS('D'), GRB_ERROR_SERIES + 8,
			5,
			Rule::Chain(1, NS('V')),
			Rule::Chain(1, NS('A')),
			Rule::Chain(1, NS('C')),
			Rule::Chain(1, NS('I')),
			Rule::Chain(1, NS('J'))
		),

		// V: VariableDeclaration -> Type ID
		Rule(NS('V'), GRB_ERROR_SERIES + 9,
			1,
			Rule::Chain(2, NS('T'), TS(LEX_ID))
		),

		// A: Assignment -> ID = Expression
		Rule(NS('A'), GRB_ERROR_SERIES + 10,
			1,
			Rule::Chain(3, TS(LEX_ID), TS(LEX_EQUAL), NS('E'))
		),

		// C: FunctionCall -> ID ( ArgumentsList )
		Rule(NS('C'), GRB_ERROR_SERIES + 11,
			1,
			Rule::Chain(4, TS(LEX_ID), TS(LEX_LEFTTHESIS), NS('R'), TS(LEX_RIGHTTHESIS))
		),

		// R: ArgumentsList -> Expression MoreArguments | empty
		Rule(NS('R'), GRB_ERROR_SERIES + 12,
			2,
			Rule::Chain(2, NS('E'), NS('Q')),
			Rule::Chain() // empty
		),

		// Q: MoreArguments -> , Expression MoreArguments | empty
		Rule(NS('Q'), GRB_ERROR_SERIES + 13,
			2,
			Rule::Chain(3, TS(LEX_COMMA), NS('E'), NS('Q')),
			Rule::Chain() // empty
		),

		// I: IfStatement -> if ( LogicalExpression ) { BlockStatements } DifferPart
		Rule(NS('I'), GRB_ERROR_SERIES + 14,
			1,
			Rule::Chain(8, TS(LEX_IF), TS(LEX_LEFTTHESIS), NS('L'), TS(LEX_RIGHTTHESIS), TS(LEX_LEFTBRACE), NS('B'), TS(LEX_BRACELET), NS('M')) // M is DifferPart
		),

		// M: DifferPart -> differ { BlockStatements } | empty
		Rule(NS('M'), GRB_ERROR_SERIES + 15,
			2,
			Rule::Chain(4, TS(LEX_DIFFER), TS(LEX_LEFTBRACE), NS('B'), TS(LEX_BRACELET)),
			Rule::Chain() // empty
		),

		// L: LogicalExpression -> Expression ComparisonOperator Expression
		Rule(NS('L'), GRB_ERROR_SERIES + 16,
			1,
			Rule::Chain(3, NS('E'), NS('U'), NS('E'))
		),

		// U: ComparisonOperator -> == | != | < | > | <= | >=
		Rule(NS('U'), GRB_ERROR_SERIES + 17,
			6,
			Rule::Chain(1, TS(LEX_ISEQUAL)),
			Rule::Chain(1, TS(LEX_NOT_EQUAL)),
			Rule::Chain(1, TS(LEX_LESS)),
			Rule::Chain(1, TS(LEX_MORE)),
			Rule::Chain(1, TS(LEX_LESS_OR_EQUAL)),
			Rule::Chain(1, TS(LEX_MORE_OR_EQUAL))
		),

		// J: BecauseLoop -> because ( InitializationForBecause ; LogicalExpression ; OperationForBecause ) { BlockStatements }
		Rule(NS('J'), GRB_ERROR_SERIES + 18,
			1,
			Rule::Chain(11, TS(LEX_BECAUSE), TS(LEX_LEFTTHESIS), NS('K'), TS(LEX_SEMICOLON), NS('L'), TS(LEX_SEMICOLON), NS('Z'), TS(LEX_RIGHTTHESIS), TS(LEX_LEFTBRACE), NS('B'), TS(LEX_BRACELET))
		),

		// K: InitializationForBecause -> Type ID = Expression | ID = Expression
		Rule(NS('K'), GRB_ERROR_SERIES + 19,
			2,
			Rule::Chain(4, NS('T'), TS(LEX_ID), TS(LEX_EQUAL), NS('E')),
			Rule::Chain(3, TS(LEX_ID), TS(LEX_EQUAL), NS('E'))
		),

		// Z: OperationForBecause -> ID UnaryOp (Inc/Dec) | ID = Expression | empty
		Rule(NS('Z'), GRB_ERROR_SERIES + 20,
			3,
			Rule::Chain(2, TS(LEX_ID), NS('N')), // N: UnaryOperator
			Rule::Chain(3, TS(LEX_ID), TS(LEX_EQUAL), NS('E')),
			Rule::Chain() // empty
		),

		// E: Expression -> Term AddOps
		Rule(NS('E'), GRB_ERROR_SERIES + 21,
			1,
			Rule::Chain(2, NS('H'), NS('O')) // H: Term, O: AddOps
		),

		// H: Term -> Factor MulOps
		Rule(NS('H'), GRB_ERROR_SERIES + 22,
			1,
			Rule::Chain(2, NS('W'), NS('_')) // W: Factor, _: MulOps
		),

		// O: AddOps -> + Term AddOps | - Term AddOps | empty
		Rule(NS('O'), GRB_ERROR_SERIES + 23,
			3,
			Rule::Chain(3, TS(LEX_PLUS), NS('H'), NS('O')),
			Rule::Chain(3, TS(LEX_MINUS), NS('H'), NS('O')),
			Rule::Chain() // empty
		),

		// _: MulOps -> * Factor MulOps | : Factor MulOps | empty
		Rule(NS('_'), GRB_ERROR_SERIES + 24,
			3,
			Rule::Chain(3, TS(LEX_STAR), NS('W'), NS('_')),
			Rule::Chain(3, TS(LEX_COLON), NS('W'), NS('_')),
			Rule::Chain() // empty
		),

		// W: Factor -> LITERAL | ID | ( Expression ) | UnaryOp Factor | FunctionCall
		Rule(NS('W'), GRB_ERROR_SERIES + 25,
			5,
			Rule::Chain(1, TS(LEX_LITERAL)),
			Rule::Chain(1, TS(LEX_ID)),
			Rule::Chain(3, TS(LEX_LEFTTHESIS), NS('E'), TS(LEX_RIGHTTHESIS)),
			Rule::Chain(2, NS('N'), NS('W')), // N: UnaryOperator
			Rule::Chain(1, NS('C')) // C: FunctionCall
		),

		// N: UnaryOperator -> ~ | ++ | --
		Rule(NS('N'), GRB_ERROR_SERIES + 26,
			3,
			Rule::Chain(1, TS(LEX_BITNOT)),
			Rule::Chain(1, TS(LEX_INCREMENT)),
			Rule::Chain(1, TS(LEX_DECREMENT))
		)
	);
}