#pragma once
#define IN_MAX_LEN_TEXT 1024*1024 // максимальный размер исходного кода
#define IN_CODE_ENDL '\n' // символ конца строки

// таблица проверки входной информации, индекс - код(Windows-1251) символа
// значения iN::F - запрещенный символ, iN::T -разрешенный символ, iN::I - игнорировать(не выводить)
// если 0 <= значение < 256 - то выводится данное значение
namespace In {
    struct iN { // исходные данные
        enum { T = 1024, F = 2048, I = 4096, S = 8192, Z = 512}; // T - допустимый символ, F - недопустимый символ, i - игнорировать, иначе заменить
        int size = 0; // размер исходного кода
        int lines = 0; // количество строк
        int ignore = 0; // количество проигнорированных символов
        unsigned char* text; // исходный код(Windows-1251)
        int code[256] = { // новая таблица проверки
            T, T, T, T, T, T, T, T, T, S, T, T, T, T, T, T, \
            T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, \
            S, T, T, T, T, Z, Z, T, Z, Z, Z, Z, Z, Z, T, Z, \
            T, T, T, T, T, T, T, T, T, T, T, Z, Z, Z, Z, T, \
            T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, \
            T, T, T, T, T, T, T, T, T, T,	T, T, T, T, T, T, \
            T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, \
            T, T, T, T, T, T, T, T, T, T, T, Z, Z, Z, T, T, \
            \
            T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, \
            T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, \
            T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, \
            T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, \
            T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, \
            T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, \
            T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, \
            T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, \
                    };
    };
    iN getin(wchar_t infile[]); // ввести и проверить входной поток
}