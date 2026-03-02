#include "../../include/esclib.h"

#include "./coredata.h"
#include "./common_utils.h"

#if defined(unix) || defined(__unix) || defined(__unix__)
	#include <unistd.h>
#elif defined(_WIN32) || defined(_WIN64)
#endif

#include <string.h>
#include <stdlib.h>

// TODO: DO return codes here
int UniWrite(UniWriteTarget target, const void* buf, size_t n) {
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

#elif defined(_WIN32) || defined(_WIN64)

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

int UniWriteLen(UniWriteTarget target, const void* buf) {
	return UniWrite(target, buf, strlen(buf));
}

size_t WriteToBackbuff(const SBCell* content, size_t cellCount) {
	if(!content) { return 0; }

	size_t maxCells = GetBackbuffCellCount();

	if(DATA.Buffers.backbuffOffset >= maxCells) { return 0; }

	if(cellCount > maxCells - DATA.Buffers.backbuffOffset) {
		cellCount = maxCells - DATA.Buffers.backbuffOffset;
	}

	size_t bytesToWrite = cellCount * sizeof(SBCell);

	memcpy(DATA.Buffers.backbuff + DATA.Buffers.backbuffOffset, content, bytesToWrite);

	DATA.Buffers.backbuffOffset += cellCount;

	return bytesToWrite;
}
