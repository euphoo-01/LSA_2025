#pragma once
#include<iostream>
#include<stack>
#include"stdafx.h"
#include"IT.h"
#include"LT.h"
#define MAX_LEX_SIZE 4096
#define DOT '.'
#define MARK '\''
#define NEW_LINE '`'
#define SEMICOLON ';'
#define COMMA ','
#define LEFT_BRACE '{'
#define RIGHT_BRACE '}'
#define LEFTTHESIS '('
#define RIGHTTHESIS ')'
#define PLUS '+'
#define MINUS '-'
#define STAR '*'
#define EQUAL '='
#define DIRSLASH '/'
#define OST '%'
#define SPACE ' '
#define BECAUSE "because"
#define IF "if"
#define DIFFER "differ"
#define TYPE 't'
#define FST_UNSIGNED FST::FST _unsigned(str,\
	9,\
	FST::NODE(1, FST::RELATION('u', 1)),\
	FST::NODE(1, FST::RELATION('n', 2)),\
	FST::NODE(1, FST::RELATION('s', 3)),\
	FST::NODE(1, FST::RELATION('i', 4)),\
	FST::NODE(1, FST::RELATION('g', 5)),\
	FST::NODE(1, FST::RELATION('n', 6)),\
	FST::NODE(1, FST::RELATION('e', 7)),\
	FST::NODE(1, FST::RELATION('d', 8)),\
	FST::NODE()\
);
#define FST_INTEGER FST::FST _integer(str,\
	8,\
	FST::NODE(1, FST::RELATION('i', 1)),\
	FST::NODE(1, FST::RELATION('n', 2)),\
	FST::NODE(1, FST::RELATION('t', 3)),\
	FST::NODE(1, FST::RELATION('e', 4)),\
	FST::NODE(1, FST::RELATION('g', 5)),\
	FST::NODE(1, FST::RELATION('e', 6)),\
	FST::NODE(1, FST::RELATION('r', 7)),\
	FST::NODE()\
);
#define FST_CHAR FST::FST _char(str,\
	5,\
	FST::NODE(1, FST::RELATION('c', 1)),\
	FST::NODE(1, FST::RELATION('h', 2)),\
	FST::NODE(1, FST::RELATION('a', 3)),\
	FST::NODE(1, FST::RELATION('r', 4)),\
	FST::NODE()\
);
#define FST_LOGIC FST::FST _logic(str,\
	6,\
	FST::NODE(1, FST::RELATION('l', 1)),\
	FST::NODE(1, FST::RELATION('o', 2)),\
	FST::NODE(1, FST::RELATION('g', 3)),\
	FST::NODE(1, FST::RELATION('i', 4)),\
	FST::NODE(1, FST::RELATION('c', 5)),\
	FST::NODE()\
);
#define FST_FUNC FST::FST _func(str,\
	5,\
	FST::NODE(1, FST::RELATION('f', 1)),\
	FST::NODE(1, FST::RELATION('u', 2)),\
	FST::NODE(1, FST::RELATION('n', 3)),\
	FST::NODE(1, FST::RELATION('c', 4)),\
	FST::NODE()\
);
#define FST_SEND FST::FST _send(str,\
	5,\
	FST::NODE(1, FST::RELATION('s', 1)),\
	FST::NODE(1, FST::RELATION('e', 2)),\
	FST::NODE(1, FST::RELATION('n', 3)),\
	FST::NODE(1, FST::RELATION('d', 4)),\
	FST::NODE()\
);
#define FST_MAIN FST::FST _main(str,\
	5,\
	FST::NODE(1, FST::RELATION('m', 1)),\
	FST::NODE(1, FST::RELATION('a', 2)),\
	FST::NODE(1, FST::RELATION('i', 3)),\
	FST::NODE(1, FST::RELATION('n', 4)),\
	FST::NODE()\
);
#define FST_WRITECH FST::FST _writech(str,\
	8,\
	FST::NODE(1, FST::RELATION('w', 1)),\
	FST::NODE(1, FST::RELATION('r', 2)),\
	FST::NODE(1, FST::RELATION('i', 3)),\
	FST::NODE(1, FST::RELATION('t', 4)),\
	FST::NODE(1, FST::RELATION('e', 5)),\
	FST::NODE(1, FST::RELATION('c', 6)),\
	FST::NODE(1, FST::RELATION('h', 7)),\
	FST::NODE()\
);
#define FST_READCH FST::FST _readch(str,\
	7,\
	FST::NODE(1, FST::RELATION('r', 1)),\
	FST::NODE(1, FST::RELATION('e', 2)),\
	FST::NODE(1, FST::RELATION('a', 3)),\
	FST::NODE(1, FST::RELATION('d', 4)),\
	FST::NODE(1, FST::RELATION('c', 5)),\
	FST::NODE(1, FST::RELATION('h', 6)),\
	FST::NODE()\
);
#define FST_TRUE FST::FST _true(str,\
	5,\
	FST::NODE(1, FST::RELATION('t', 1)),\
	FST::NODE(1, FST::RELATION('r', 2)),\
	FST::NODE(1, FST::RELATION('u', 3)),\
	FST::NODE(1, FST::RELATION('e', 4)),\
	FST::NODE()\
);
#define FST_FALSE FST::FST _false(str,\
	6,\
	FST::NODE(1, FST::RELATION('f', 1)),\
	FST::NODE(1, FST::RELATION('a', 2)),\
	FST::NODE(1, FST::RELATION('l', 3)),\
	FST::NODE(1, FST::RELATION('s', 4)),\
	FST::NODE(1, FST::RELATION('e', 5)),\
	FST::NODE()\
);
#define FST_IF FST::FST _if(str,\
	3,\
	FST::NODE(1, FST::RELATION('i', 1)),\
	FST::NODE(1, FST::RELATION('f', 2)),\
	FST::NODE()\
);
#define FST_DIFFER FST::FST _differ(str,\
	7,\
	FST::NODE(1, FST::RELATION('d', 1)),\
	FST::NODE(1, FST::RELATION('i', 2)),\
	FST::NODE(1, FST::RELATION('f', 3)),\
	FST::NODE(1, FST::RELATION('f', 4)),\
	FST::NODE(1, FST::RELATION('e', 5)),\
	FST::NODE(1, FST::RELATION('r', 6)),\
	FST::NODE()\
);
#define FST_BECAUSE FST::FST _because(str,\
	8,\
	FST::NODE(1, FST::RELATION('b', 1)),\
	FST::NODE(1, FST::RELATION('e', 2)),\
	FST::NODE(1, FST::RELATION('c', 3)),\
	FST::NODE(1, FST::RELATION('a', 4)),\
	FST::NODE(1, FST::RELATION('u', 5)),\
	FST::NODE(1, FST::RELATION('s', 6)),\
	FST::NODE(1, FST::RELATION('e', 7)),\
	FST::NODE()\
);
#define FST_UNSIGNED_INTEGER FST::FST _unsigned_integer(str, \
    17, \
    FST::NODE(1, FST::RELATION('u', 1)), \
    FST::NODE(1, FST::RELATION('n', 2)), \
    FST::NODE(1, FST::RELATION('s', 3)), \
    FST::NODE(1, FST::RELATION('i', 4)), \
    FST::NODE(1, FST::RELATION('g', 5)), \
    FST::NODE(1, FST::RELATION('n', 6)), \
    FST::NODE(1, FST::RELATION('e', 7)), \
    FST::NODE(1, FST::RELATION('d', 8)), \
    FST::NODE(1, FST::RELATION(' ', 9)), \
    FST::NODE(1, FST::RELATION('i', 10)), \
    FST::NODE(1, FST::RELATION('n', 11)), \
    FST::NODE(1, FST::RELATION('t', 12)), \
    FST::NODE(1, FST::RELATION('e', 13)), \
    FST::NODE(1, FST::RELATION('g', 14)), \
    FST::NODE(1, FST::RELATION('e', 15)), \
    FST::NODE(1, FST::RELATION('r', 16)), \
    FST::NODE() \
);
#define FST_LITERAL FST::FST _literal(str,\
	3,\
	FST::NODE(21,\
		FST::RELATION('0', 0), FST::RELATION('1', 0), FST::RELATION('2', 0),\
		FST::RELATION('3', 0), FST::RELATION('4', 0), FST::RELATION('5', 0),\
		FST::RELATION('6', 0), FST::RELATION('7', 0), FST::RELATION('8', 0),\
		FST::RELATION('9', 0), FST::RELATION('.', 1), FST::RELATION('0', 2), FST::RELATION('1', 2), FST::RELATION('2', 2),\
		FST::RELATION('3', 2), FST::RELATION('4', 2), FST::RELATION('5', 2),\
		FST::RELATION('6', 2), FST::RELATION('7', 2), FST::RELATION('8', 2),\
		FST::RELATION('9', 2)),\
		FST::NODE(20,\
		FST::RELATION('0', 1), FST::RELATION('1', 1), FST::RELATION('2', 1),\
		FST::RELATION('3', 1), FST::RELATION('4', 1), FST::RELATION('5', 1),\
		FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1),\
		FST::RELATION('9', 1), FST::RELATION('0', 2), FST::RELATION('1', 2), FST::RELATION('2', 2),\
		FST::RELATION('3', 2), FST::RELATION('4', 2), FST::RELATION('5', 2),\
		FST::RELATION('6', 2), FST::RELATION('7', 2), FST::RELATION('8', 2),\
		FST::RELATION('9', 2)),\
	FST::NODE()\
);
#define FST_IDEN FST::FST _iden(str, 3, \
	FST::NODE(53, \
		FST::RELATION('a', 1), FST::RELATION('b', 1), FST::RELATION('c', 1), FST::RELATION('d', 1), FST::RELATION('e', 1), FST::RELATION('f', 1), FST::RELATION('g', 1), FST::RELATION('h', 1), FST::RELATION('i', 1), FST::RELATION('j', 1), FST::RELATION('k', 1), FST::RELATION('l', 1), FST::RELATION('m', 1), FST::RELATION('n', 1), FST::RELATION('o', 1), FST::RELATION('p', 1), FST::RELATION('q', 1), FST::RELATION('r', 1), FST::RELATION('s', 1), FST::RELATION('t', 1), FST::RELATION('u', 1), FST::RELATION('v', 1), FST::RELATION('w', 1), FST::RELATION('x', 1), FST::RELATION('y', 1), FST::RELATION('z', 1), \
		FST::RELATION('A', 1), FST::RELATION('B', 1), FST::RELATION('C', 1), FST::RELATION('D', 1), FST::RELATION('E', 1), FST::RELATION('F', 1), FST::RELATION('G', 1), FST::RELATION('H', 1), FST::RELATION('I', 1), FST::RELATION('J', 1), FST::RELATION('K', 1), FST::RELATION('L', 1), FST::RELATION('M', 1), FST::RELATION('N', 1), FST::RELATION('O', 1), FST::RELATION('P', 1), FST::RELATION('Q', 1), FST::RELATION('R', 1), FST::RELATION('S', 1), FST::RELATION('T', 1), FST::RELATION('U', 1), FST::RELATION('V', 1), FST::RELATION('W', 1), FST::RELATION('X', 1), FST::RELATION('Y', 1), FST::RELATION('Z', 1), \
		FST::RELATION('_', 1) \
	), \
	FST::NODE(126, \
		FST::RELATION('a', 1), FST::RELATION('b', 1), FST::RELATION('c', 1), FST::RELATION('d', 1), FST::RELATION('e', 1), FST::RELATION('f', 1), FST::RELATION('g', 1), FST::RELATION('h', 1), FST::RELATION('i', 1), FST::RELATION('j', 1), FST::RELATION('k', 1), FST::RELATION('l', 1), FST::RELATION('m', 1), FST::RELATION('n', 1), FST::RELATION('o', 1), FST::RELATION('p', 1), FST::RELATION('q', 1), FST::RELATION('r', 1), FST::RELATION('s', 1), FST::RELATION('t', 1), FST::RELATION('u', 1), FST::RELATION('v', 1), FST::RELATION('w', 1), FST::RELATION('x', 1), FST::RELATION('y', 1), FST::RELATION('z', 1), \
		FST::RELATION('A', 1), FST::RELATION('B', 1), FST::RELATION('C', 1), FST::RELATION('D', 1), FST::RELATION('E', 1), FST::RELATION('F', 1), FST::RELATION('G', 1), FST::RELATION('H', 1), FST::RELATION('I', 1), FST::RELATION('J', 1), FST::RELATION('K', 1), FST::RELATION('L', 1), FST::RELATION('M', 1), FST::RELATION('N', 1), FST::RELATION('O', 1), FST::RELATION('P', 1), FST::RELATION('Q', 1), FST::RELATION('R', 1), FST::RELATION('S', 1), FST::RELATION('T', 1), FST::RELATION('U', 1), FST::RELATION('V', 1), FST::RELATION('W', 1), FST::RELATION('X', 1), FST::RELATION('Y', 1), FST::RELATION('Z', 1), \
		FST::RELATION('_', 1), \
		FST::RELATION('0', 1), FST::RELATION('1', 1), FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1), FST::RELATION('5', 1), FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1), FST::RELATION('9', 1), \
		FST::RELATION('a', 2), FST::RELATION('b', 2), FST::RELATION('c', 2), FST::RELATION('d', 2), FST::RELATION('e', 2), FST::RELATION('f', 2), FST::RELATION('g', 2), FST::RELATION('h', 2), FST::RELATION('i', 2), FST::RELATION('j', 2), FST::RELATION('k', 2), FST::RELATION('l', 2), FST::RELATION('m', 2), FST::RELATION('n', 2), FST::RELATION('o', 2), FST::RELATION('p', 2), FST::RELATION('q', 2), FST::RELATION('r', 2), FST::RELATION('s', 2), FST::RELATION('t', 2), FST::RELATION('u', 2), FST::RELATION('v', 2), FST::RELATION('w', 2), FST::RELATION('x', 2), FST::RELATION('y', 2), FST::RELATION('z', 2), \
		FST::RELATION('A', 2), FST::RELATION('B', 2), FST::RELATION('C', 2), FST::RELATION('D', 2), FST::RELATION('E', 2), FST::RELATION('F', 2), FST::RELATION('G', 2), FST::RELATION('H', 2), FST::RELATION('I', 2), FST::RELATION('J', 2), FST::RELATION('K', 2), FST::RELATION('L', 2), FST::RELATION('M', 2), FST::RELATION('N', 2), FST::RELATION('O', 2), FST::RELATION('P', 2), FST::RELATION('Q', 2), FST::RELATION('R', 2), FST::RELATION('S', 2), FST::RELATION('T', 2), FST::RELATION('U', 2), FST::RELATION('V', 2), FST::RELATION('W', 2), FST::RELATION('X', 2), FST::RELATION('Y', 2), FST::RELATION('Z', 2), \
		FST::RELATION('_', 2), \
		FST::RELATION('0', 2), FST::RELATION('1', 2), FST::RELATION('2', 2), FST::RELATION('3', 2), FST::RELATION('4', 2), FST::RELATION('5', 2), FST::RELATION('6', 2), FST::RELATION('7', 2), FST::RELATION('8', 2), FST::RELATION('9', 2) \
	), \
	FST::NODE() \
);

namespace Lex {
    struct LEX {
        IT::IdTable idTable;
        LT::LexTable lexTable;
        map<string, vector<IT::IDDATATYPE> > functions;
    };

    char FST(char *str);

    void Add_LT(int line, char token);

    LEX LexA(Parm::Parm parm, In::IN in);
}
