#include "../stdafx.h"
#include "../Headers/Error.h"
#include "../Headers/Parm.h"

namespace Parm {
    PARm getparm(int argc, wchar_t* argv[]) { // получение параметров
        PARm parm;
        bool in = 0, out = 0, log = 0;
        for (int i = 1; i < argc; i++) {
            if (wcslen(argv[i]) > PARM_MAX_SIZE) { // если превышен допустимый размер параметра
                throw ERROR_THROW(101); // вызываем исключение
            }
            if (wcsstr(argv[i], PARM_IN)) { // если задан in
                std::wcscpy(parm.in, argv[i] + wcslen(PARM_IN)); // то записываем его в нашу структуру
                in = 1; // помечаем, что он есть
            }
            if (wcsstr(argv[i], PARM_OUT)) { // если задан out
                std::wcscpy(parm.out, argv[i] + wcslen(PARM_OUT));// то записываем его в нашу структуру
                out = 1; // помечаем, что он есть
            }
            if (wcsstr(argv[i], PARM_LOG)) { // если задан log
                std::wcscpy(parm.log, argv[i] + wcslen(PARM_LOG));// то записываем его в нашу структуру
                log = 1; // помечаем, что он есть
            }
        }
        if (!in) { // если параметр in не задан то вызываем исключение
            throw ERROR_THROW(100);
        }
        if (!out) { // если out не задан, то формируем его из названия in
            std::wcscpy(parm.out, parm.in);
            std::wcscat(parm.out, PARM_OUT_DEFAULT_EXT);
        }
        if (!log) {// если log не задан, то формируем его из названия in
            std::wcscpy(parm.log, parm.in);
            std::wcscat(parm.log, PARM_LOG_DEFAULT_EXT);
        }
        return parm;
    }
}