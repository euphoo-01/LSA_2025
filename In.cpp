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
    IN getin(wchar_t infile[]) {
        IN in_result;
        in_result.size = 0;
        in_result.lines = 0;
        std::ifstream file;
        char narrow_infile[300];
        wcstombs(narrow_infile, infile, 300);
        file.open(narrow_infile);
        if (!file.is_open()) {
            throw ERROR_THROW(110);
        }
        in_result.text = new unsigned char[IN_MAX_LEN_TEXT];
        char *tmp = new char[IN_MAX_LEN_TEXT];

        while (file.getline(tmp, 1000)) {
            for (int position = 0; position < strlen(tmp); position++) {
                switch (in_result.code[int((unsigned char) tmp[position])]) {
                    case IN::T: {
                        in_result.text[in_result.size] = (unsigned) tmp[position];
                        in_result.size++;
                        break;
                    }
                    case IN::Z: {
                        in_result.text[in_result.size] = (unsigned) tmp[position];
                        in_result.size++;
                        break;
                    }
                    case IN::S: {
                        if (position == 0) {
                            while (tmp[position] == SPACE || tmp[position] == TAB) {
                                position++;
                            }
                            position--;
                            break;
                        }
                        if (in_result.code[int((unsigned char) tmp[position - 1])] == IN::Z || in_result.code[int(
                                (unsigned char) tmp[position + 1])] == IN::Z) {
                            in_result.ignore++;
                            break;
                        }
                        if (in_result.code[int((unsigned char) tmp[position + 1])] == IN::S || in_result.code[int(
                                (unsigned char) tmp[position - 1])] == IN::S) {
                            in_result.ignore++;
                            break;
                        }
                        in_result.text[in_result.size] = (unsigned) tmp[position];
                        in_result.size++;
                        break;
                    }
                    default:
                        in_result.text[in_result.size] = in_result.code[tmp[position]];
                        in_result.size++;
                }
            }
            in_result.lines++;
            in_result.text[in_result.size] = '`';
            in_result.size++;
        }
        in_result.text[in_result.size] = '\0';
        return in_result;
    }
}
