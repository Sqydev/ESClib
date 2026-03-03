#include "../../include/esclib.h"

#include "./coredata.h"
#include "./common_utils.h"

#if defined(unix) || defined(__unix) || defined(__unix__)
	#include <unistd.h>
	#include <fcntl.h>
	#include <termios.h>
#elif defined(_WIN32) || defined(_WIN64)
	#include <windows.h>
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



void EnableRawMode(void) {
#if defined(unix) || defined(__unix) || defined(__unix__)

	// NOTE:
	// Syntax explaining!:
	// | is add:
	// 1000
	// 0010
	// ----
	// 1010
	// ~ == NOT gate:
	// 1010 -> 0101
	// & == AND Gate
	
	struct termios workspace;

	// NOTE: Get corrent terminal config
	if(tcgetattr(STDIN_FILENO, &DATA.TuiData.oldTerminalConfig) == -1) {
		UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: Enable raw mode failed at tcgetattr\n");
		exit(EXIT_FAILURE);
	}
	workspace = DATA.TuiData.oldTerminalConfig;

	// NOTE: Turn off cannonic mode and echo. (So read() doesn't need enter)
	workspace.c_lflag &= ~(ICANON | ECHO);

	// NOTE: I don't fucking know
	workspace.c_iflag &= ~(ICRNL | INLCR);

	// NOTE: Read() returns asap
	workspace.c_cc[VMIN] = 0;
	workspace.c_cc[VTIME] = 0;

	// NOTE: Apply NOW
	if(tcsetattr(STDIN_FILENO, TCSANOW, &workspace) == -1) {
		UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: Enable raw mode failed at secnd tcgetattr\n");
		exit(EXIT_FAILURE);
	}

	int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
	
	// NOTE: Read doesn't block nor wait(-1 if no data)
	if(fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK) == -1) {
		UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: Enable raw mode failed at fcntl\n");
		exit(EXIT_FAILURE);
	}

	DATA.TuiData.escTerminalConfig = workspace;
	
#elif defined(_WIN32) || defined(_WIN64)

	
    DATA.TuiData.hStdin = GetStdHandle(STD_INPUT_HANDLE);

    if (DATA.TuiData.hStdin == INVALID_HANDLE_VALUE) {
        UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: GetStdHandle failed\n");
        exit(EXIT_FAILURE);
    }

    if (!GetConsoleMode(DATA.TuiData.hStdin, &DATA.TuiData.oldConsoleMode)) {
        UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: GetConsoleMode failed\n");
        exit(EXIT_FAILURE);
    }

    DWORD rawMode = DATA.TuiData.oldConsoleMode;

    // wyłącz tryb linii, echo i przetwarzanie Ctrl+C
    rawMode &= ~(ENABLE_LINE_INPUT |
                 ENABLE_ECHO_INPUT |
                 ENABLE_PROCESSED_INPUT);

    if (!SetConsoleMode(DATA.TuiData.hStdin, rawMode)) {
        UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: SetConsoleMode failed\n");
        exit(EXIT_FAILURE);
    }
	
#endif
}

void DisableRawMode(void) {
#if defined(unix) || defined(__unix) || defined(__unix__)

	if(tcsetattr(STDIN_FILENO, TCSANOW, &DATA.TuiData.oldTerminalConfig) == -1) {
		UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: Disable raw mode failed at tcgetattr\n");
		exit(EXIT_FAILURE);
	}

	int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
	if(fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK) == -1) {
		UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: Disable raw mode failed at fcntl\n");
		exit(EXIT_FAILURE);
	}

#elif defined(_WIN32) || defined(_WIN64)

	 if (!SetConsoleMode(DATA.TuiData.hStdin,
                        DATA.TuiData.oldConsoleMode)) {
        UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: Restore console mode failed\n");
        exit(EXIT_FAILURE);
    }

#endif
}
