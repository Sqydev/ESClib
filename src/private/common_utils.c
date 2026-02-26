#include "../../include/esclib.h"
#include <stdlib.h>

#include "./common_utils.h"

#if defined(unix) || defined(__unix) || defined(__unix__)
	#include <unistd.h>
#elif defined(__WIN32) || defined(__WIN64)
#endif

// TODO: DO return codes here
int UniWrite(UniWriteTarget target, const void *buf, size_t n) {
#if defined(unix) || defined(__unix) || defined(__unix__)

	switch((int)target) {
		case UNI_WRITE_TARGET_STDOUT: {
			return (int)write(STDOUT_FILENO, buf, n);
		}
		case UNI_WRITE_TARGET_STDERR: {
			return (int)write(STDERR_FILENO, buf, n);
		}
		default: {
			return EXIT_FAILURE;
		}
	}

#elif defined(__WIN32) || defined(__WIN64)

	HANDLE h;
	switch (target) {
		case UNI_WRITE_TARGET_STDOUT:
			h = GetStdHandle(STD_OUTPUT_HANDLE);
			break;
		case UNI_WRITE_TARGET_STDERR:
			h = GetStdHandle(STD_ERROR_HANDLE);
			break;
		default:
			return -1;
	}
	DWORD written = 0;
	if (!WriteFile(h, buf, (DWORD)n, &written, NULL)) return -1;
			
	return (int)written;

#endif
}
