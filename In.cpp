#include"stdafx.h"
#include"In.h"
#include"Error.h"
#include"Out.h"
#include"Parm.h"
#include"Log.h"
#include <cstring>
#include <cstdlib>

#define SPACE ' '
#define TAB '\t'

namespace In {
    IN getin(wchar_t infile[]) { // ввод
        IN in_result;
        in_result.size = 0;
        in_result.lines = 0;
        std::ifstream file;
        char narrow_infile[300];
        wcstombs(narrow_infile, infile, 300);
        file.open(narrow_infile);
        if (!file.is_open()) {
            throw ERROR_THROW(10);
        }
        in_result.text = new unsigned char[IN_MAX_LEN_TEXT];
        char *tmp = new char[IN_MAX_LEN_TEXT];

        while (file.getline(tmp, 1000)) {
            int len = strlen(tmp);
            for (int position = 0; position < len; position++) {

                // обработка комментариев
                if (tmp[position] == '/' && position + 1 < len && tmp[position + 1] == '/') {
                    break;
                }
                
                // просто копируем символ
                in_result.text[in_result.size] = (unsigned char)tmp[position];
                in_result.size++;
            }
            in_result.lines++;
            in_result.text[in_result.size] = '`';
            in_result.size++;
        }
        in_result.text[in_result.size] = '\0';
        return in_result;
    }
}