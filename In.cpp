#include "stdafx.h"
#include "In.h"
#include "Error.h"
#include "Out.h"
#include "Parm.h"
#include "Log.h"
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>

namespace In {
    IN getin(wchar_t infile[]) { // ввод
        IN in_result;
        in_result.size = 0;
        in_result.lines = 0;
        in_result.ignore = 0;

        std::ifstream file;
        char narrow_infile[300];
        wcstombs(narrow_infile, infile, 300);
        file.open(narrow_infile);

        if (!file.is_open()) {
            throw ERROR_THROW(100);
        }

        in_result.text = new unsigned char[IN_MAX_LEN_TEXT];
        char *tmp = new char[IN_MAX_LEN_TEXT];

        while (file.getline(tmp, IN_MAX_LEN_TEXT)) {
            int len = strlen(tmp);
            for (int position = 0; position < len; position++) {

                // обработка комментариев
                if (tmp[position] == '/' && position + 1 < len && tmp[position + 1] == '/') {
                    break;
                }
                
                unsigned char c = (unsigned char)tmp[position];
                int type = in_result.code[c];
                
                if (type == IN::T) {
                    in_result.text[in_result.size] = c;
                    in_result.size++;
                } else if (type == IN::F) {
                    throw ERROR_THROW_IN(111, in_result.lines + 1, position + 1);
                } else if (type == IN::I) {
                    in_result.ignore++;
                }
            }
            in_result.lines++;
            in_result.text[in_result.size] = IN_CODE_ENDL;
            in_result.size++;
        }
        in_result.text[in_result.size] = '\0';
        
        file.close();
        delete[] tmp;
        
        return in_result;
    }
}
