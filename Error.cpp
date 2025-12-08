#include"stdafx.h"
#include"Error.h"

// 0-99: Системные ошибки
// 100-110: Ошибки параметров
// 111-125: Лексические ошибки
// 300-350: Семантические ошибки
// 600-610: Синтаксические ошибки

namespace Error {
    ERROR errors[ERROR_MAX_ENTRY] = {
        ERROR_ENTRY(0, "Недопустимый код ошибки"),
        ERROR_ENTRY(1, "Системный сбой"),
        ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
        ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
        
        // 10-99: System/IO/Limits
        ERROR_ENTRY(10, "Ошибка при открытии файла с исходным кодом (-in)"),
        ERROR_ENTRY_NODEF(11),
        ERROR_ENTRY(12, "Ошибка при создании файла с протоколом (-log)"),
        ERROR_ENTRY(13, "Ошибка при создании выходного файла с расширением .out/.asm"),
        ERROR_ENTRY(14, "Недопустимый размер таблицы лексем"),
        ERROR_ENTRY(15, "Превышен размер таблицы лексем"),
        ERROR_ENTRY_NODEF(16),
        ERROR_ENTRY(17, "Недопустимый размер таблицы идентификаторов"),
        ERROR_ENTRY(18, "Превышен размер таблицы идентификаторов"),
        ERROR_ENTRY(19, "Выход за границы таблицы идентификаторов"),
        
        ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40),
        ERROR_ENTRY_NODEF10(50), ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80),
        ERROR_ENTRY_NODEF10(90),

        // 100-110: Params
        ERROR_ENTRY(100, "Параметр -in должен быть задан"),
        ERROR_ENTRY(101, "Превышена длина входного параметра"),
        ERROR_ENTRY_NODEF(102), ERROR_ENTRY_NODEF(103), ERROR_ENTRY_NODEF(104), ERROR_ENTRY_NODEF(105),
        ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107), ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
        ERROR_ENTRY_NODEF(110),

        // 111-125: Lexical
        ERROR_ENTRY(111, "Недопустимый символ в исходном файле (-in)"),
        ERROR_ENTRY_NODEF(112), ERROR_ENTRY_NODEF(113), ERROR_ENTRY_NODEF(114), ERROR_ENTRY_NODEF(115),
        ERROR_ENTRY_NODEF(116), ERROR_ENTRY_NODEF(117), ERROR_ENTRY_NODEF(118),
        ERROR_ENTRY(119, "Превышена длина лексемы"),
        ERROR_ENTRY_NODEF(120), ERROR_ENTRY_NODEF(121), ERROR_ENTRY_NODEF(122), ERROR_ENTRY_NODEF(123),
        ERROR_ENTRY_NODEF(124),
        ERROR_ENTRY(125, "Ошибка в строковом литерале"),

        ERROR_ENTRY_NODEF10(126), ERROR_ENTRY_NODEF10(136), ERROR_ENTRY_NODEF10(146), ERROR_ENTRY_NODEF10(156),
        ERROR_ENTRY_NODEF10(166), ERROR_ENTRY_NODEF10(176), ERROR_ENTRY_NODEF10(186), ERROR_ENTRY_NODEF10(196),
        ERROR_ENTRY_NODEF100(200),

        // 300-350: Semantic
        ERROR_ENTRY(300, "Неопределенная семантическая ошибка"),
        ERROR_ENTRY_NODEF(301),
        ERROR_ENTRY(302, "Все функции должны быть объявлены до main"),
        ERROR_ENTRY(303, "Функция main не объявлена"),
        ERROR_ENTRY(304, "Ключевое слово не может быть идентификатором"),
        ERROR_ENTRY(305, "Дублирование идентификатора"),
        ERROR_ENTRY(306, "Идентификатор не определён"),
        ERROR_ENTRY(307, "Функция уже определена"),
        ERROR_ENTRY(308, "Превышено максимально возможное число параметров в функции"),
        ERROR_ENTRY(309, "Повторное объявление main"),
        ERROR_ENTRY(310, "Присваиваемое значение не соответствует типу переменной"),
        ERROR_ENTRY(311, "Возвращаемое значение в функции не соответствует ее типу"),
        ERROR_ENTRY(312, "Фактические и формальные параметры не соответствуют по типу или количеству"),
        ERROR_ENTRY(313, "При присваивании, слева должна быть переменная или должен быть параметр"),
        ERROR_ENTRY(314, "Недопустимо деление на ноль"),
        ERROR_ENTRY(315, "Функция не может быть строкового типа"),
        ERROR_ENTRY(316, "Некорректный тип данных"),
        ERROR_ENTRY(317, "Ошибка в параметрах функции (превышено количество)"),
        ERROR_ENTRY(318, "Ошибка в параметрах вызываемой функции (недостаточно параметров)"),
        ERROR_ENTRY(319, "Ошибка в логическом выражении (не boolean)"),
        ERROR_ENTRY(320, "Переменная уже объявлена"), // Reserve if needed

        ERROR_ENTRY_NODEF10(321), ERROR_ENTRY_NODEF10(331), ERROR_ENTRY_NODEF10(341),
        
        ERROR_ENTRY_NODEF100(351), ERROR_ENTRY_NODEF100(451), ERROR_ENTRY_NODEF10(551), ERROR_ENTRY_NODEF10(561),
        ERROR_ENTRY_NODEF10(571), ERROR_ENTRY_NODEF10(581), ERROR_ENTRY_NODEF10(591),

        // 600-610: Syntax
        ERROR_ENTRY(600, "Неверная структура программы"),
        ERROR_ENTRY(601, "Ошибочный оператор"),
        ERROR_ENTRY(602, "Ошибка в выражении"),
        ERROR_ENTRY(603, "Синтаксическая ошибка (параметры)"), // Reused/Renamed
        ERROR_ENTRY(604, "Синтаксическая ошибка (параметры вызова)"), // Reused/Renamed
        ERROR_ENTRY(605, "Ошибка в подвыражении"),
        ERROR_ENTRY(606, "Синтаксическая ошибка (условие)"), // Reused/Renamed
        ERROR_ENTRY(607, "Ошибка в логическом подвыражении"),
        
        ERROR_ENTRY_NODEF100(700),ERROR_ENTRY_NODEF100(800),
        ERROR_ENTRY_NODEF100(900)
    };

    ERROR geterror(int id) {
        if (id > 0 && id < ERROR_MAX_ENTRY) {
            for (int i = 0; i < 1000; i++) {
                if (id == errors[i].id) {
                    return errors[i];
                }
            }
            return errors[0];
        } else {
            return errors[0];
        }
    }

    ERROR geterrorin(int id, int line = -1, int col = -1) {
        if (id > 0 && id < ERROR_MAX_ENTRY) {
            for (int i = 0; i < 1000; i++) {
                if (id == errors[i].id) {
                    errors[i].inext.col = col;
                    errors[i].inext.line = line;
                    return errors[i];
                }
            }
        } else {
            return errors[0];
        }
    }
};