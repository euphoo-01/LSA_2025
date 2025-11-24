#pragma once

#define PARM_IN  L"-in:" // ключ для файла исходного кода
#define PARM_OUT L"-out:" // ключ для файла объектного кода
#define PARM_LOG L"-log:" // ключ для файла журнала
#define PARM_MAX_SIZE 300 // максимальная длина строки параметра
#define PARM_OUT_DEFAULT_EXT L".asm" //расширение объектного кода по умолчанию
#define PARM_LOG_DEFAULT_EXT L".log" //расширение файла журнала по умолчанию

namespace Parm {
    struct PARm { // входные параметры
        wchar_t in[PARM_MAX_SIZE]; // имя файла исходного кода
        wchar_t out[PARM_MAX_SIZE]; // имя файла объектного кода
        wchar_t log[PARM_MAX_SIZE]; // имя log файла
    };
    PARm getparm(int argc, wchar_t* argv[]); // функция для получения параметров
}