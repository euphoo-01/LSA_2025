#include"../stdafx.h"
#include"../Headers/Error.h"
namespace Error
{
		ERROr errors[ERROR_MAX_ENTRY] = {
		ERROR_ENTRY(0,"Недопустимый код ошибки"),
		ERROR_ENTRY(1,"Системный сбой"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40),
		ERROR_ENTRY_NODEF10(50), ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80),ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100,"Параметр -in должен быть задан"),
		ERROR_ENTRY(101, "Превышена длина входного параметра"), 
		ERROR_ENTRY(102, "Все функции должны быть объявлены до main"),
		ERROR_ENTRY(103, "Функция main не объявлена"),
		ERROR_ENTRY(104, "Ключевое слово не может быть идентификатором"),
		ERROR_ENTRY(105,"Дублирование идентификатора"),
		ERROR_ENTRY(106,"Идентификатор не определён"),
		ERROR_ENTRY(107, "Функция уже определена"),
		ERROR_ENTRY(108, "Превышено максимально возможное число параметров в функции"),
		ERROR_ENTRY(109, "Повторное объявление main"),
		ERROR_ENTRY(110,"Ошибка при открытии файла с исходным кодом (-in)"),
		ERROR_ENTRY(111,"Недопустимый символ в исходном файле (-in)"),
		ERROR_ENTRY(112,"Ошибка при создании файла с протоколом (-log)"),
		ERROR_ENTRY(113,"Ошибка при создании выходного файла с расширением .out"),
		ERROR_ENTRY(114,"Недопустимый размер таблицы лексем"),
		ERROR_ENTRY(115,"Превышен размер таблицы лексем"),
		ERROR_ENTRY(117,"Недопустимый размер таблицы идентификаторов"),
		ERROR_ENTRY(118,"Превышен размер таблицы идентификаторов"),
		ERROR_ENTRY(119,"Превышена длина лексемы"),
		ERROR_ENTRY(120, "Присваиваемое значение не соответствует типу переменной"),
		ERROR_ENTRY(121, "Возвращаемое значение в функции не соответствует ее типу"),
		ERROR_ENTRY(122, "Фактические и формальные параметры не соответствуют по типу или количеству"),
		ERROR_ENTRY(123, "При присваивании, слева должна быть переменная или должен быть параметр"),
		ERROR_ENTRY(124, "Недопустимо деление на ноль"),
		ERROR_ENTRY(125, "Ошибка в строковом литерале"),
		ERROR_ENTRY(126, "Функция не может быть строкового типа"),
		ERROR_ENTRY_NODEF10(130),ERROR_ENTRY_NODEF10(140),ERROR_ENTRY_NODEF10(150),ERROR_ENTRY_NODEF10(160),
		ERROR_ENTRY_NODEF10(170),ERROR_ENTRY_NODEF10(180),ERROR_ENTRY_NODEF10(190),ERROR_ENTRY_NODEF100(200),ERROR_ENTRY_NODEF100(300),
		ERROR_ENTRY_NODEF100(400),ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY(600, "Неверная структура программы"),
		ERROR_ENTRY(601, "Ошибочный оператор"),
		ERROR_ENTRY(602, "Ошибка в выражении"),
		ERROR_ENTRY(603, "Ошибка в параметрах функции"),
		ERROR_ENTRY(604, "Ошибка в параметрах вызываемой функции"),
		ERROR_ENTRY(605, "Ошибка в подвыражении"),
		ERROR_ENTRY(606, "Ошибка в логическом выражении"),
		ERROR_ENTRY(607, "Ошибка в логическом подвыражении"),
		ERROR_ENTRY_NODEF100(700),ERROR_ENTRY_NODEF100(800),
		ERROR_ENTRY_NODEF100(900)
	};
	ERROr geterror(int id)
	{
		if (id > 0 && id < ERROR_MAX_ENTRY)
		{
			for (int i = 0; i < 1000; i++) {
				if (id == errors[i].id) {
					return errors[i];
				}
			}
			return errors[0];
		}
		else
		{
			return errors[0];
		}
	}
	ERROr geterrorin(int id, int line = -1, int col = -1)
	{
		if (id > 0 && id < ERROR_MAX_ENTRY)
		{
			for (int i = 0; i < 1000; i++) {
				if (id == errors[i].id) {
					errors[i].inext.col = col;
					errors[i].inext.line = line;
					return errors[i];
				}
			}
		}
		else
		{
			return errors[0];
		}
		return errors[0];
	}
};