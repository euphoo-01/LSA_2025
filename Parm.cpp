#include "stdafx.h"
#include "Error.h"
#include "Parm.h"
#include <cwchar>

namespace Parm {
	Parm getParm(int argc, wchar_t* argv[]) { // разбор параметров запуска
		Parm parm;
		parm.build = false;
		bool in = 0, out = 0, log = 0;
		for (int i = 1; i < argc; i++) {
			if (wcslen(argv[i]) > PARM_MAX_SIZE) {
				throw ERROR_THROW(101);
			}
			if (wcsstr(argv[i], PARM_IN)) {
				wcscpy(parm.in, argv[i] + wcslen(PARM_IN));
				in = 1;
			}
			if (wcsstr(argv[i], PARM_OUT)) {
				wcscpy(parm.out, argv[i] + wcslen(PARM_OUT));
				out = 1;
			}
			if (wcsstr(argv[i], PARM_LOG)) {
				wcscpy(parm.log, argv[i] + wcslen(PARM_LOG));
				log = 1;
			}
			if (wcsstr(argv[i], PARM_BUILD)) {
				parm.build = true;
			}
		}
		if (!in) {
			throw ERROR_THROW(100);
		}
		if (!out) { // по умолчанию out = in.asm
			wcscpy(parm.out, parm.in);
			wcscat(parm.out, PARM_OUT_DEFAULT_EXT);
		}
		if (!log) { // по умолчанию log = in.log
			wcscpy(parm.log, parm.in);
			wcscat(parm.log, PARM_LOG_DEFAULT_EXT);
		}
		return parm;
	}
}