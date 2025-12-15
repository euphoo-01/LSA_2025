#pragma once
#define IN_MAX_LEN_TEXT 1024*1024 // максимальный размер исходного кода
#define IN_CODE_ENDL '\n' // символ конца строки

// таблица проверки входной информации, индекс - код(Windows-1251) символа
// IN::F - запрещенный символ, IN::T -разрешенный символ, IN::I - игнорировать
// если 0 <= значение < 256 - то выводится данное значение
namespace In {
    struct IN {
        // исходные данные
        enum { T = 1024, F = 2048, I = 4096 };

        // T - допустимый символ, F - недопустимый символ, I - игнорировать
        int size = 0; // размер исходного кода
        int lines = 0; // количество строк
        int ignore = 0; // количество проигнорированных символов
        unsigned char *text; // исходный код(Windows-1251)
        int code[256] = {
            // таблица проверки
            F, F, F, F, F, F, F, F, F, T, T, F, F, I, F, F,
            F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F,
            T, T, F, F, F, T, T, T, T, T, T, T, T, T, T, T,
            T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T,
            F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T,
            T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T,
            T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T,
            T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F,
            T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T,
            T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T,
            T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T,
            T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T,
            T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T,
            T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T,
            T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T,
            T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T
        };
    };

    IN getin(wchar_t infile[]); // ввести и проверить входной поток
}