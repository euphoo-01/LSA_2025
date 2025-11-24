#include "../stdafx.h"
#include"../Headers/In.h"
#include"../Headers/Error.h"
#include"../Headers/Out.h"
#include"../Headers/Parm.h"
#include"../Headers/Log.h"
#include <cstring>
#include <cstdlib>

#define SPACE ' '
#define TAB '\t'

namespace In
{
    iN getin(wchar_t infile[])
    {
        iN in_result;
        in_result.size = 0;
        in_result.lines = 0;
        std::ifstream file;

        int filenameLen = wcslen(infile);
        char filename[filenameLen + 1];
        wcstombs(filename, infile, filenameLen);
        filename[filenameLen] = '\0';

        file.open(filename);
        if (!file.is_open())
        {
            throw ERROR_THROW(110);
        }
        in_result.text = new unsigned char[IN_MAX_LEN_TEXT];
        char* tmp = new char[IN_MAX_LEN_TEXT];

        while (file.getline(tmp, 1000))
        {
            for (int position = 0; position < strlen(tmp); position++)
            {
                switch (in_result.code[int((unsigned char)tmp[position])])
                {
                    case iN::T: {
                        in_result.text[in_result.size] = (unsigned)tmp[position];
                        in_result.size++;
                        break;
                    }
                    case iN::Z: {
                        in_result.text[in_result.size] = (unsigned)tmp[position];
                        in_result.size++;
                        break;
                    }
                    case iN::S: {
                        if (position == 0) {
                            while (tmp[position] == SPACE || tmp[position] == TAB)
                            {
                                position++;
                            }
                            position--;
                            break;
                        }
                        if (in_result.code[int((unsigned char)tmp[position - 1])] == iN::Z || in_result.code[int((unsigned char)tmp[position + 1])] == iN::Z) {
                            in_result.ignore++;
                            break;
                        }
                        if (in_result.code[int((unsigned char)tmp[position + 1])] == iN::S || in_result.code[int((unsigned char)tmp[position - 1])] == iN::S) {
                            in_result.ignore++;
                            break;
                        }
                        in_result.text[in_result.size] = (unsigned)tmp[position];
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