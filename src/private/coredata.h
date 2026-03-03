#ifndef ESCLIB_PRIVATE_COREDATA_H
#define ESCLIB_PRIVATE_COREDATA_H

#if defined(unix) || defined(__unix) || defined(__unix__)
	#include <termios.h>
#elif defined(_WIN32) || defined(__WIN64)
#endif

#include "../../include/esclib.h"

#include <stdbool.h>
#include <signal.h>

typedef struct {
	struct {
		SBCell* frontbuff;
		
		SBCell* backbuff;
		size_t backbuffOffset;
	} Buffers;

	struct {
		Vector2i termdimm;
		Vector2i termdimmInPixels;
		
		bool initiated; // TUI MUST DIE
		
#if defined(unix) || defined(__unix) || defined(__unix__)

		struct termios oldTerminalConfig;
		struct termios escTerminalConfig;

#elif defined(_WIN32) || defined(_WIN64)

		DWORD oldConsoleMode;
		HANDLE hStdin;

#endif
	} TuiData;

	struct {
		int targetFps;
		double target;
	} Time;

	struct {
		struct {
#if defined(unix) || defined(__unix) || defined(__unix__)

			struct sigaction old;

#elif defined(_WIN32) || defined(_WIN64)
#endif
			volatile sig_atomic_t triggered;
			bool enabled;
			bool enabledESClibTasks;
			bool enabledCustomTasks;
			bool enabledBuildInTasks;
			void (**customTasks)(void);
			int customTasksNumber;
		} SIG_INT;

		struct {
#if defined(unix) || defined(__unix) || defined(__unix__)

			struct sigaction old;

#elif defined(_WIN32) || defined(_WIN64)
#endif
			volatile sig_atomic_t triggered;
			bool enabled;
			bool enabledESClibTasks;
			bool enabledCustomTasks;
			bool enabledBuildInTasks;
			void (**customTasks)(void);
			int customTasksNumber;
		} SIG_WINCH;
	} SignalData;
} CoreData;

extern CoreData DATA;

#endif
