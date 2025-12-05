#include <iostream>
#include <cstdarg>
#include <cstring>
#include "FST.h"
using namespace std;

namespace FST { // пространство имен для конечного автомата
	RELATION::RELATION(char c, short nn) { // конструктор отношений вершин
		symbol = c; // символ перехода
		nnode = nn; // номер смежной вершины
	}

	NODE::NODE() { // конструктор для узла по умолчанию
		n_relation = 0; // количество связей
		RELATION* relations = nullptr; // массив связей
	}
	NODE::NODE(short n, RELATION re1, ...) { // конструктор для узла с параметрами
		n_relation = n; // задаем число связей

		relations = new RELATION[n]; // выделяем память под массив связей
		// Инициализация списка аргументов
		va_list args;
		va_start(args, re1); // Начинаем список аргументов с re1
		// Сохраняем первый элемент
		relations[0] = re1;
		// Заполняем массив оставшимися элементами
		for (int i = 1; i < n; i++) {
			relations[i] = va_arg(args, RELATION); // Получаем следующий RELATION из списка аргументов
		}
		// Завершаем работу с аргументами
		va_end(args);
	}

	FST::FST(const char* s, short ns, NODE n, ...) { // конструктор конечного автомата
		strin = s; // сохраняем переданную строку
		nstates = ns; // количество состояний
		nodes = new NODE[ns]; // выделяем память для массива узлов
		// Инициализация списка аргументов
		va_list args;
		va_start(args, n); // Начинаем список аргументов с n
		// Сохраняем первый элемент
		nodes[0] = n;
		// Заполняем массив оставшимися элементами
		for (int k = 1; k < ns; k++) {
			nodes[k] = va_arg(args, NODE); // Получаем следующий NODE из списка аргументов
		}
		// Завершаем работу с аргументами
		va_end(args);
		rstates = new short[nstates]; // выделяем память для массива состояний
		memset(rstates, 0xff, sizeof(short) * nstates); // заполняем его значениями 0xff
		rstates[0] = 0; // устанавливаем начальное значение
		position = -1; // задаем текущую позицию
	}

	bool step(FST& fst, short*& rstates) { // один шаг автомата
		bool rc = false; // флажок для отслеживания корректного выполнения
		swap(rstates, fst.rstates); // меняем местами массивы
		for (short i = 0; i < fst.nstates; i++) { // проходим все состояния
			if (rstates[i] == fst.position) { // если текущее состояние совпало
				for (short j = 0; j < fst.nodes[i].n_relation; j++) { // проходим по всем отношениям узла
					if (fst.nodes[i].relations[j].symbol == fst.strin[fst.position]) { // проверяем соответствует ли символ текущей позиции в строке
						fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1; // если да, то обновляем состояние
						rc = true; // отмечаем, что успешно
					}
				}
			}
		}
		return rc;
	}

	bool execute(FST& fst) { // функция для распознавания цепочки
		short* rstates = new short[fst.nstates]; // выделяем память для буферного массива состояний
		memset(rstates, 0xff, sizeof(short) * fst.nstates); // заполняем его 0xff
		short lstring = strlen(fst.strin);// берем длину входной строки
		bool rc = true; // флажок для корректности выполнения
		for (short i = 0; i < lstring && rc; i++) { // проходим по каждому символу
			fst.position++; // увеличиваем позицию
			rc = step(fst, rstates); // вызываем step для текущего состояния
		}
		delete[] rstates; // очищаем память
		/*for (int i = 0; i < fst.nstates; i++) {
			cout << fst.rstates[i] << ' ';
		}*/
		return (rc ? (fst.rstates[fst.nstates - 1] == lstring) : rc); // вернет true, только если rc true и длина входной цепочки = значению в последнем состоянии
	}
}