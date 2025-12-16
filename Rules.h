#pragma once
#include "GRB.h"
#include "LT.h"
#define NS(n) GRB::Rule::Chain::N(n)
#define TS(n) GRB::Rule::Chain::T(n)
#define ISNS(n) GRB::Rule::Chain::isN(n)
#define GRB_ERROR_SERIES 600

namespace GRB {
    Greibach greibach(NS('S'), TS('$'),
                      6,
                      // S (Start)
                      Rule(NS('S'), GRB_ERROR_SERIES + 0,
                           3,
                           // main { ... };
                           Rule::Chain(6, TS(LEX_MAIN), TS(LEX_LEFTBRACE), NS('N'),
                                       TS(LEX_BRACELET), TS(LEX_SEMICOLON), TS('$')),

                           // func ... { ... }; S
                           Rule::Chain(11, TS(LEX_FUNC), TS(LEX_UNSIGNED_INTEGER), TS(LEX_ID),
                                       TS(LEX_LEFTTHESIS), NS('F'), TS(LEX_RIGHTTHESIS), TS(LEX_LEFTBRACE), NS('N'),
                                       TS(LEX_BRACELET), TS(LEX_SEMICOLON), NS('S')),

                           // func ... () { ... }; S
                           Rule::Chain(10, TS(LEX_FUNC), TS(LEX_UNSIGNED_INTEGER), TS(LEX_ID),
                                       TS(LEX_LEFTTHESIS), TS(LEX_RIGHTTHESIS), TS(LEX_LEFTBRACE), NS('N'),
                                       TS(LEX_BRACELET),
                                       TS(LEX_SEMICOLON), NS('S'))
                      ),

                      // N Инструкции
                      Rule(NS('N'), GRB_ERROR_SERIES + 1,
                           31,

                           // send E ;
                           Rule::Chain(3, TS(LEX_SEND), NS('E'), TS(LEX_SEMICOLON)),

                           // объявление (type id ...)
                           Rule::Chain(6, TS(LEX_UNSIGNED_INTEGER), TS(LEX_ID), TS(LEX_EQUAL), NS('E'),
                                       TS(LEX_SEMICOLON), NS('N')),
                           Rule::Chain(5, TS(LEX_UNSIGNED_INTEGER), TS(LEX_ID), TS(LEX_EQUAL), NS('E'),
                                       TS(LEX_SEMICOLON)),
                           Rule::Chain(4, TS(LEX_UNSIGNED_INTEGER), TS(LEX_ID), TS(LEX_SEMICOLON), NS('N')),
                           Rule::Chain(3, TS(LEX_UNSIGNED_INTEGER), TS(LEX_ID), TS(LEX_SEMICOLON)),

                           // присваивание (id = ...)
                           Rule::Chain(5, TS(LEX_ID), TS(LEX_EQUAL), NS('E'), TS(LEX_SEMICOLON), NS('N')),
                           Rule::Chain(4, TS(LEX_ID), TS(LEX_EQUAL), NS('E'), TS(LEX_SEMICOLON)),

                           // вызов процедуры (id (...);)
                           Rule::Chain(6, TS(LEX_ID), TS(LEX_LEFTTHESIS), NS('W'), TS(LEX_RIGHTTHESIS),
                                       TS(LEX_SEMICOLON), NS('N')),
                           Rule::Chain(5, TS(LEX_ID), TS(LEX_LEFTTHESIS), NS('W'), TS(LEX_RIGHTTHESIS),
                                       TS(LEX_SEMICOLON)),
                           // без аргументов
                           Rule::Chain(5, TS(LEX_ID), TS(LEX_LEFTTHESIS), TS(LEX_RIGHTTHESIS), TS(LEX_SEMICOLON),
                                       NS('N')),
                           Rule::Chain(4, TS(LEX_ID), TS(LEX_LEFTTHESIS), TS(LEX_RIGHTTHESIS), TS(LEX_SEMICOLON)),

                           // инкремент / декремент
                           Rule::Chain(4, TS(LEX_ID), TS(LEX_INC), TS(LEX_SEMICOLON), NS('N')),
                           Rule::Chain(3, TS(LEX_ID), TS(LEX_INC), TS(LEX_SEMICOLON)),
                           Rule::Chain(4, TS(LEX_ID), TS(LEX_DEC), TS(LEX_SEMICOLON), NS('N')),
                           Rule::Chain(3, TS(LEX_ID), TS(LEX_DEC), TS(LEX_SEMICOLON)),

                           // writech
                           Rule::Chain(6, TS(LEX_WRITECH), TS(LEX_LEFTTHESIS), NS('E'), TS(LEX_RIGHTTHESIS),
                                       TS(LEX_SEMICOLON), NS('N')),
                           Rule::Chain(5, TS(LEX_WRITECH), TS(LEX_LEFTTHESIS), NS('E'), TS(LEX_RIGHTTHESIS),
                                       TS(LEX_SEMICOLON)),

                           // if
                           Rule::Chain(14, TS(LEX_IF), TS(LEX_LEFTTHESIS), NS('E'), TS(LEX_RIGHTTHESIS),
                                       TS(LEX_LEFTBRACE), NS('N'), TS(LEX_BRACELET), TS(LEX_SEMICOLON), TS(LEX_DIFFER),
                                       TS(LEX_LEFTBRACE), NS('N'), TS(LEX_BRACELET), TS(LEX_SEMICOLON), NS('N')),
                           Rule::Chain(13, TS(LEX_IF), TS(LEX_LEFTTHESIS), NS('E'), TS(LEX_RIGHTTHESIS),
                                       TS(LEX_LEFTBRACE), NS('N'), TS(LEX_BRACELET), TS(LEX_SEMICOLON), TS(LEX_DIFFER),
                                       TS(LEX_LEFTBRACE), NS('N'), TS(LEX_BRACELET), TS(LEX_SEMICOLON)),
                           Rule::Chain(9, TS(LEX_IF), TS(LEX_LEFTTHESIS), NS('E'), TS(LEX_RIGHTTHESIS),
                                       TS(LEX_LEFTBRACE), NS('N'), TS(LEX_BRACELET), TS(LEX_SEMICOLON), NS('N')),
                           Rule::Chain(8, TS(LEX_IF), TS(LEX_LEFTTHESIS), NS('E'), TS(LEX_RIGHTTHESIS),
                                       TS(LEX_LEFTBRACE), NS('N'), TS(LEX_BRACELET), TS(LEX_SEMICOLON)),

                           // because
                           Rule::Chain(17, TS(LEX_BECAUSE), TS(LEX_LEFTTHESIS), TS(LEX_UNSIGNED_INTEGER), TS(LEX_ID),
                                       TS(LEX_EQUAL), NS('E'), TS(LEX_SEMICOLON), NS('E'), TS(LEX_SEMICOLON),
                                       TS(LEX_ID), TS(LEX_INC), TS(LEX_RIGHTTHESIS), TS(LEX_LEFTBRACE), NS('N'),
                                       TS(LEX_BRACELET), TS(LEX_SEMICOLON), NS('N')),
                           Rule::Chain(16, TS(LEX_BECAUSE), TS(LEX_LEFTTHESIS), TS(LEX_UNSIGNED_INTEGER), TS(LEX_ID),
                                       TS(LEX_EQUAL), NS('E'), TS(LEX_SEMICOLON), NS('E'), TS(LEX_SEMICOLON),
                                       TS(LEX_ID), TS(LEX_INC), TS(LEX_RIGHTTHESIS), TS(LEX_LEFTBRACE), NS('N'),
                                       TS(LEX_BRACELET), TS(LEX_SEMICOLON)),
                           Rule::Chain(17, TS(LEX_BECAUSE), TS(LEX_LEFTTHESIS), TS(LEX_UNSIGNED_INTEGER), TS(LEX_ID),
                                       TS(LEX_EQUAL), NS('E'), TS(LEX_SEMICOLON), NS('E'), TS(LEX_SEMICOLON),
                                       TS(LEX_ID), TS(LEX_DEC), TS(LEX_RIGHTTHESIS), TS(LEX_LEFTBRACE), NS('N'),
                                       TS(LEX_BRACELET), TS(LEX_SEMICOLON), NS('N')),
                           Rule::Chain(16, TS(LEX_BECAUSE), TS(LEX_LEFTTHESIS), TS(LEX_UNSIGNED_INTEGER), TS(LEX_ID),
                                       TS(LEX_EQUAL), NS('E'), TS(LEX_SEMICOLON), NS('E'), TS(LEX_SEMICOLON),
                                       TS(LEX_ID), TS(LEX_DEC), TS(LEX_RIGHTTHESIS), TS(LEX_LEFTBRACE), NS('N'),
                                       TS(LEX_BRACELET), TS(LEX_SEMICOLON)),

                           Rule::Chain(16, TS(LEX_BECAUSE), TS(LEX_LEFTTHESIS), TS(LEX_ID), TS(LEX_EQUAL), NS('E'),
                                       TS(LEX_SEMICOLON), NS('E'), TS(LEX_SEMICOLON), TS(LEX_ID), TS(LEX_INC),
                                       TS(LEX_RIGHTTHESIS), TS(LEX_LEFTBRACE), NS('N'), TS(LEX_BRACELET),
                                       TS(LEX_SEMICOLON), NS('N')),
                           Rule::Chain(15, TS(LEX_BECAUSE), TS(LEX_LEFTTHESIS), TS(LEX_ID), TS(LEX_EQUAL), NS('E'),
                                       TS(LEX_SEMICOLON), NS('E'), TS(LEX_SEMICOLON), TS(LEX_ID), TS(LEX_INC),
                                       TS(LEX_RIGHTTHESIS), TS(LEX_LEFTBRACE), NS('N'), TS(LEX_BRACELET),
                                       TS(LEX_SEMICOLON)),
                           Rule::Chain(16, TS(LEX_BECAUSE), TS(LEX_LEFTTHESIS), TS(LEX_ID), TS(LEX_EQUAL), NS('E'),
                                       TS(LEX_SEMICOLON), NS('E'), TS(LEX_SEMICOLON), TS(LEX_ID), TS(LEX_DEC),
                                       TS(LEX_RIGHTTHESIS), TS(LEX_LEFTBRACE), NS('N'), TS(LEX_BRACELET),
                                       TS(LEX_SEMICOLON), NS('N')),
                           Rule::Chain(15, TS(LEX_BECAUSE), TS(LEX_LEFTTHESIS), TS(LEX_ID), TS(LEX_EQUAL), NS('E'),
                                       TS(LEX_SEMICOLON), NS('E'), TS(LEX_SEMICOLON), TS(LEX_ID), TS(LEX_DEC),
                                       TS(LEX_RIGHTTHESIS), TS(LEX_LEFTBRACE), NS('N'), TS(LEX_BRACELET),
                                       TS(LEX_SEMICOLON)),

                           Rule::Chain(18, TS(LEX_BECAUSE), TS(LEX_LEFTTHESIS), TS(LEX_ID), TS(LEX_EQUAL), NS('E'),
                                       TS(LEX_SEMICOLON), NS('E'), TS(LEX_SEMICOLON), TS(LEX_ID), TS(LEX_EQUAL),
                                       NS('E'), TS(LEX_RIGHTTHESIS), TS(LEX_LEFTBRACE), NS('N'), TS(LEX_BRACELET),
                                       TS(LEX_SEMICOLON), NS('N')),
                           Rule::Chain(17, TS(LEX_BECAUSE), TS(LEX_LEFTTHESIS), TS(LEX_ID), TS(LEX_EQUAL), NS('E'),
                                       TS(LEX_SEMICOLON), NS('E'), TS(LEX_SEMICOLON), TS(LEX_ID), TS(LEX_EQUAL),
                                       NS('E'), TS(LEX_RIGHTTHESIS), TS(LEX_LEFTBRACE), NS('N'), TS(LEX_BRACELET),
                                       TS(LEX_SEMICOLON))
                      ),

                      // F Params
                      Rule(NS('F'), GRB_ERROR_SERIES + 4,
                           2,
                           Rule::Chain(4, TS(LEX_UNSIGNED_INTEGER), TS(LEX_ID), TS(LEX_COMMA), NS('F')),
                           Rule::Chain(2, TS(LEX_UNSIGNED_INTEGER), TS(LEX_ID))
                      ),

                      // W Args
                      Rule(NS('W'), GRB_ERROR_SERIES + 5,
                           26,
                           Rule::Chain(7, TS(LEX_ID), TS(LEX_LEFTTHESIS), NS('W'), TS(LEX_RIGHTTHESIS), NS('M'),
                                       TS(LEX_COMMA), NS('W')),
                           Rule::Chain(6, TS(LEX_ID), TS(LEX_LEFTTHESIS), NS('W'), TS(LEX_RIGHTTHESIS), TS(LEX_COMMA),
                                       NS('W')),
                           Rule::Chain(5, TS(LEX_ID), TS(LEX_LEFTTHESIS), NS('W'), TS(LEX_RIGHTTHESIS), NS('M')),
                           Rule::Chain(4, TS(LEX_ID), TS(LEX_LEFTTHESIS), NS('W'), TS(LEX_RIGHTTHESIS)),

                           // ID ( ) ...
                           Rule::Chain(6, TS(LEX_ID), TS(LEX_LEFTTHESIS), TS(LEX_RIGHTTHESIS), NS('M'), TS(LEX_COMMA),
                                       NS('W')),
                           Rule::Chain(5, TS(LEX_ID), TS(LEX_LEFTTHESIS), TS(LEX_RIGHTTHESIS), TS(LEX_COMMA), NS('W')),
                           Rule::Chain(4, TS(LEX_ID), TS(LEX_LEFTTHESIS), TS(LEX_RIGHTTHESIS), NS('M')),
                           Rule::Chain(3, TS(LEX_ID), TS(LEX_LEFTTHESIS), TS(LEX_RIGHTTHESIS)),

                           // ID ...
                           Rule::Chain(4, TS(LEX_ID), NS('M'), TS(LEX_COMMA), NS('W')),
                           Rule::Chain(2, TS(LEX_ID), NS('M')),
                           Rule::Chain(3, TS(LEX_ID), TS(LEX_COMMA), NS('W')),
                           Rule::Chain(1, TS(LEX_ID)),

                           // начинается с литерала
                           Rule::Chain(4, TS(LEX_LITERAL), NS('M'), TS(LEX_COMMA), NS('W')),
                           Rule::Chain(2, TS(LEX_LITERAL), NS('M')),
                           Rule::Chain(3, TS(LEX_LITERAL), TS(LEX_COMMA), NS('W')),
                           Rule::Chain(1, TS(LEX_LITERAL)),

                           // начинается с (
                           Rule::Chain(6, TS(LEX_LEFTTHESIS), NS('E'), TS(LEX_RIGHTTHESIS), NS('M'), TS(LEX_COMMA),
                                       NS('W')),
                           Rule::Chain(5, TS(LEX_LEFTTHESIS), NS('E'), TS(LEX_RIGHTTHESIS), TS(LEX_COMMA), NS('W')),
                           Rule::Chain(4, TS(LEX_LEFTTHESIS), NS('E'), TS(LEX_RIGHTTHESIS), NS('M')),
                           Rule::Chain(3, TS(LEX_LEFTTHESIS), NS('E'), TS(LEX_RIGHTTHESIS)),

                           // начинается с readch
                           Rule::Chain(6, TS(LEX_READCH), TS(LEX_LEFTTHESIS), TS(LEX_RIGHTTHESIS), NS('M'),
                                       TS(LEX_COMMA), NS('W')),
                           Rule::Chain(5, TS(LEX_READCH), TS(LEX_LEFTTHESIS), TS(LEX_RIGHTTHESIS), TS(LEX_COMMA),
                                       NS('W')),
                           Rule::Chain(4, TS(LEX_READCH), TS(LEX_LEFTTHESIS), TS(LEX_RIGHTTHESIS), NS('M')),
                           Rule::Chain(3, TS(LEX_READCH), TS(LEX_LEFTTHESIS), TS(LEX_RIGHTTHESIS)),

                           // начинается с ~
                           Rule::Chain(4, TS(LEX_BIT_NOT), NS('E'), TS(LEX_COMMA), NS('W')),
                           Rule::Chain(2, TS(LEX_BIT_NOT), NS('E'))
                      ),

                      // E Expressions
                      Rule(NS('E'), GRB_ERROR_SERIES + 2,
                           13,
                           // вызов функции: ID ( W ) ...
                           Rule::Chain(5, TS(LEX_ID), TS(LEX_LEFTTHESIS), NS('W'), TS(LEX_RIGHTTHESIS), NS('M')),
                           Rule::Chain(4, TS(LEX_ID), TS(LEX_LEFTTHESIS), NS('W'), TS(LEX_RIGHTTHESIS)),
                           // без аргументов
                           Rule::Chain(4, TS(LEX_ID), TS(LEX_LEFTTHESIS), TS(LEX_RIGHTTHESIS), NS('M')),
                           Rule::Chain(3, TS(LEX_ID), TS(LEX_LEFTTHESIS), TS(LEX_RIGHTTHESIS)),

                           // идентификатор: ID ...
                           Rule::Chain(2, TS(LEX_ID), NS('M')),
                           Rule::Chain(1, TS(LEX_ID)),

                           // литерал: LIT ...
                           Rule::Chain(2, TS(LEX_LITERAL), NS('M')),
                           Rule::Chain(1, TS(LEX_LITERAL)),

                           // скобки: ( E ) ...
                           Rule::Chain(4, TS(LEX_LEFTTHESIS), NS('E'), TS(LEX_RIGHTTHESIS), NS('M')),
                           Rule::Chain(3, TS(LEX_LEFTTHESIS), NS('E'), TS(LEX_RIGHTTHESIS)),

                           // readch
                           Rule::Chain(4, TS(LEX_READCH), TS(LEX_LEFTTHESIS), TS(LEX_RIGHTTHESIS), NS('M')),
                           Rule::Chain(3, TS(LEX_READCH), TS(LEX_LEFTTHESIS), TS(LEX_RIGHTTHESIS)),

                           // инверсия
                           Rule::Chain(2, TS(LEX_BIT_NOT), NS('E'))
                      ),

                      // M Math
                      Rule(NS('M'), GRB_ERROR_SERIES + 3,
                           11,
                           Rule::Chain(2, TS(LEX_PLUS), NS('E')),
                           Rule::Chain(2, TS(LEX_MINUS), NS('E')),
                           Rule::Chain(2, TS(LEX_STAR), NS('E')),
                           Rule::Chain(2, TS(LEX_COLON), NS('E')),
                           Rule::Chain(2, TS(LEX_OST), NS('E')),
                           Rule::Chain(2, TS(LEX_MORE_OR_EQUAL), NS('E')),
                           Rule::Chain(2, TS(LEX_LESS_OR_EQUAL), NS('E')),
                           Rule::Chain(2, TS(LEX_ISEQUAL), NS('E')),
                           Rule::Chain(2, TS(LEX_NOT_EQUAL), NS('E')),
                           Rule::Chain(2, TS(LEX_MORE), NS('E')),
                           Rule::Chain(2, TS(LEX_LESS), NS('E'))
                      )
    );
}
