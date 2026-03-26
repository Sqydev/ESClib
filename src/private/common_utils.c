/*
* Copyright (c) 2025-present Wojciech Kaptur ( _Sqyd_ / Sqydev )
* Github: https://github.com/Sqydev
* GPG Fingerprint: 6DC2516B0DFDA9C59661650722F7B8A777F33B56
* 
* This software is provided "as-is", without any express or implied warranty. In no event
* will the authors be held liable for any damages arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose, including commercial
* applications, and to alter it and redistribute it freely, subject to the following restrictions:
* 
* 1. Non-Misrepresentation: The origin of this software must not be misrepresented; 
*    you must not claim that you wrote the original software. An acknowledgment in 
*    product documentation is appreciated but not required.
* 
* 2. Source-Level Copyleft: Any altered versions (forks) of this software's source code, 
*    or files containing significant portions of this code, must be distributed under 
*    these same license terms. Such modified source code must be made publicly available 
*    to any recipient, even if used over a network (SaaS).
* 
* 3. Proprietary Integration: This software may be integrated into, linked with, or 
*    used as a component of proprietary and closed-source products. In such cases, 
*    the surrounding proprietary application code does not need to be disclosed, 
*    provided that the original or modified source code of THIS software remains 
*    available under the terms of Section 2.
* 
* 4. Persistent Metadata: All original credits, including those in the source code headers 
*    and binary metadata (e.g., ELF .comment section, PE StringFileInfo, or equivalent), 
*    must not be removed. You may add your own credits to forks, provided the original 
*    authorship remains clearly identified.
* 
* 5. Notice Retention: This license notice may not be removed or altered from any 
*    source or binary distribution.
*/

#include "../../include/esclib.h"

#include "./coredata.h"
#include "./common_utils.h"

#if defined(unix) || defined(__unix) || defined(__unix__)
	#include <unistd.h>
	#include <fcntl.h>
	#include <termios.h>
	#include <errno.h>
#elif defined(_WIN32) || defined(_WIN64)
	#include <windows.h>
#endif

#include <string.h>
#include <stdlib.h>

// TODO: DO return codes here
size_t UniWrite(UniWriteTarget target, const void* buf, size_t n) {
#if defined(unix) || defined(__unix) || defined(__unix__)

	int fd = (target == UNI_WRITE_TARGET_STDOUT) ? STDOUT_FILENO : STDERR_FILENO;
	size_t total_written = 0;
	const char* ptr = (const char*)buf;

	while(total_written < n) {
		ssize_t written = write(fd, ptr + total_written, n - total_written);

		if (written == -1) {
			if(errno == EINTR) {
				continue;
			}
			if(errno == EAGAIN || errno == EWOULDBLOCK) {
				continue;
			}
			return -1;
		}
		total_written += written;
	}

	return total_written;

#elif defined(_WIN32) || defined(_WIN64)

	HANDLE h = (target == UNI_WRITE_TARGET_STDOUT) ? GetStdHandle(STD_OUTPUT_HANDLE) : GetStdHandle(STD_ERROR_HANDLE);
    if (h == INVALID_HANDLE_VALUE) return -1;

    DWORD total_written = 0;
    const char* ptr = (const char*)buf;

    while (total_written < n) {
        DWORD written = 0;
        if (!WriteFile(h, ptr + total_written, (DWORD)(n - total_written), &written, NULL)) {
            return -1;
        }
        total_written += written;
    }
    return (int)total_written;

#endif
}

// TODO: LIBCSITTYFNSINDEPENDENCE
// strlen
size_t UniWriteLen(UniWriteTarget target, const void* buf) {
	return UniWrite(target, buf, strlen(buf));
}

void WriteToBackbuff(const SBCell cell, size_t x, size_t y) {
	if(x > (size_t)DATA.TuiData.termdimm.x * DATA.TuiData.termdimm.y) {
		UniWriteLen(UNI_WRITE_TARGET_STDERR, "WARNING: X is out of bounce\n");
		return;
	}
	if(y > (size_t)DATA.TuiData.termdimm.y) {
		UniWriteLen(UNI_WRITE_TARGET_STDERR, "WARNING: Y is out of bounce\n");
		return;
	}

    size_t index = y * DATA.TuiData.termdimm.x + x;

	if(index > DATA.Buffers.backbuffOffset) { DATA.Buffers.backbuffOffset = index; }

    DATA.Buffers.backbuff[index] = cell;
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
