#include "Build.h"
#include "Log.h"
#include "Error.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <iomanip>

using namespace std;

namespace Build {
    // путь к стандартной библиотеке
    const std::string STD_LIB_PATH = "/home/euphoo/02. University/КП/LSA_2025/LSA_StdLib/build/libLSA_StdLib.a";

    // выполнение сборки
    void Build(Parm::Parm parm) {
        if (!parm.build) return;

        char nasm_cmd[1024];
        char link_cmd[1024];
        char out_asm[300];

        wcstombs(out_asm, parm.out, 300);
        
        // .asm -> .o
        std::string obj_file = out_asm;
        size_t lastdot = obj_file.find_last_of(".");
        if (lastdot == std::string::npos) obj_file += ".o";
        else obj_file = obj_file.substr(0, lastdot) + ".o";
        
        // .asm -> .bin
        std::string exe_file = out_asm;
        lastdot = exe_file.find_last_of(".");
        if (lastdot != std::string::npos) exe_file = exe_file.substr(0, lastdot);
        exe_file += ".bin";

        // команды сборки
        sprintf(nasm_cmd, "nasm -f elf64 \"%s\" -o \"%s\"", out_asm, obj_file.c_str());
        sprintf(link_cmd, "g++ -o \"%s\" \"%s\" \"%s\"", exe_file.c_str(), obj_file.c_str(), STD_LIB_PATH.c_str());

        cout << "Сборка..." << endl;
        cout << nasm_cmd << endl;
        
        if (std::system(nasm_cmd) == 0) {
            cout << link_cmd << endl;
            if (std::system(link_cmd) == 0) {
                cout << "Сборка выполнена успешно. Исполняемый файл: " << exe_file << endl;
                cout << "Запуск программы..." << endl;
                cout << setfill('-') << setw(40) << "" << endl;
                char open_cmd[302] = "./";
                std::strcpy(open_cmd, exe_file.c_str());
                std::system(open_cmd);
                cout << endl;
            } else {
                cout << "Ошибка линковки" << endl;
            }
        } else {
            cout << "Ошибка ассемблирования" << endl;
        }
    }
}