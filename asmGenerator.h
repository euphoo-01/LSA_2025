#pragma once
#include "LexA.h"

#define HEAD ".586\n.MODEL FLAT, stdcall\nincludelib kernel32.lib\nincludelib user32.lib\nincludelib libucrt.lib\nincludelib msvcrtd.lib\nincludelib \"..\\Debug\\MyLib.lib\""

#define PROTOS "\ncompnum PROTO :DWORD, :DWORD, :DWORD, :DWORD, :DWORD\nprintss PROTO :DWORD, :DWORD\nprintssln PROTO :DWORD, :DWORD\nreadss PROTO : DWORD, : DWORD\ncompare PROTO : DWORD, : DWORD\nlen PROTO : DWORD\ngetmin PROTO : DWORD, : DWORD\ngetmax PROTO : DWORD, : DWORD\nExitProcess PROTO : DWORD\nSetConsoleOutputCP PROTO : DWORD\nSetConsoleCP PROTO : DWORD\nwaitPause PROTO\n\n.STACK 4096\n\n"

#define END "theend:\n\tcall waitPause\n\tpush 0\n\tcall ExitProcess\nSOMETHINGWRONG:\n\tpush OFFSET null_division\n\tpush 4\n\tcall printss\njmp THEENDER\nERRORREAD:\n\tpush OFFSET error_read\n\tpush 4\n\tcall printss\nTHEENDER:\n\tcall waitPause\n\tpush -1\n\tcall ExitProcess\n"

#define FOOTER "\nmain ENDP\nend main"


namespace ASMGenerator
{
	void asmGenerator(LexA::LEX lext, wchar_t outfile[]);
}