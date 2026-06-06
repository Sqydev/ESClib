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

#include "../include/esclib.h"

#include "./private/coredata.h"
#include "./private/common_utils.h"
#include "signal.h"
#include <signal.h>

#if defined(unix) || defined(__unix) || defined(__unix__)
	#include <unistd.h>
#elif defined(_WIN32) || defined(_WIN64)
#endif

#include <errno.h>
#include <stdlib.h>

// TODO: Make returns not be EXIT_SUCCESS or EXIT_FAILURE
int EnableSignal(int signal) {
	switch(signal) {
		case SIGINT: {
			DATA.SignalData.SIG_INT.enabled = true;
			break;
		}

		case SIGWINCH: {
			DATA.SignalData.SIG_WINCH.enabled = true;
			break;
		}
		
		default: {
			TraceLog("[ESCLIB.EnableSignal]: ERROR: Couldn't enable signal becouse signal is NOT supported");
			errno = EINVAL;
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

// TODO: Make returns not be EXIT_SUCCESS or EXIT_FAILURE
int DisableSignal(int signal) {
	switch(signal) {
		case SIGINT: {
			DATA.SignalData.SIG_INT.enabled = false;
			break;
		}

		case SIGWINCH: {
			DATA.SignalData.SIG_WINCH.enabled = false;
			break;
		}
		
		default: {
			TraceLog("[ESCLIB.DisableSignal]: ERROR: Couldn't disable signal becouse signal is NOT supported");
			errno = EINVAL;
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

// TODO: Make returns not be EXIT_SUCCESS or EXIT_FAILURE
int EnableSignalESClibTasks(int signal) {
	switch(signal) {
		case SIGINT: {
			DATA.SignalData.SIG_INT.enabledESClibTasks = true;
			break;
		}

		case SIGWINCH: {
			DATA.SignalData.SIG_WINCH.enabledESClibTasks = true;
			break;
		}
		
		default: {
			TraceLog("[ESCLIB.EnableSignalESClibTasks]: ERROR: Couldn't enable ESClib task becouse signal is NOT supported");
			errno = EINVAL;
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

// TODO: Make returns not be EXIT_SUCCESS or EXIT_FAILURE
int DisableSignalESClibTasks(int signal) {
	switch(signal) {
		case SIGINT: {
			DATA.SignalData.SIG_INT.enabledESClibTasks = false;
			break;
		}

		case SIGWINCH: {
			DATA.SignalData.SIG_WINCH.enabledESClibTasks = false;
			break;
		}
		
		default: {
			TraceLog("[ESCLIB.DisableSignalESClibTasks]: ERROR: Couldn't disable ESClib task becouse signal is NOT supported");
			errno = EINVAL;
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

// TODO: Make returns not be EXIT_SUCCESS or EXIT_FAILURE
int EnableSignalCustomTasks(int signal) {
	switch(signal) {
		case SIGINT: {
			DATA.SignalData.SIG_INT.enabledCustomTasks = true;
			break;
		}

		case SIGWINCH: {
			DATA.SignalData.SIG_WINCH.enabledCustomTasks = true;
			break;
		}
		
		default: {
			TraceLog("[ESCLIB.EnableSignalCustomTasks]: ERROR: Couldn't enable custom task becouse signal is NOT supported");
			errno = EINVAL;
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

// TODO: Make returns not be EXIT_SUCCESS or EXIT_FAILURE
int DisableSignalCustomTasks(int signal) {
	switch(signal) {
		case SIGINT: {
			DATA.SignalData.SIG_INT.enabledCustomTasks = false;
			break;
		}

		case SIGWINCH: {
			DATA.SignalData.SIG_WINCH.enabledCustomTasks = false;
			break;
		}
		
		default: {
			TraceLog("[ESCLIB.DisableSignalCustomTasks]: ERROR: Couldn't disable custom task becouse signal is NOT supported");
			errno = EINVAL;
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

// TODO: Make returns not be EXIT_SUCCESS or EXIT_FAILURE
int EnableSignalBuildInTasks(int signal) {
	switch(signal) {
		case SIGINT: {
			DATA.SignalData.SIG_INT.enabledBuildInTasks = true;
			break;
		}

		case SIGWINCH: {
			DATA.SignalData.SIG_WINCH.enabledBuildInTasks = true;
			break;
		}
		
		default: {
			TraceLog("[ESCLIB.EnableSignalBuildInTasks]: ERROR: Couldn't enable build in task becouse signal is NOT supported");
			errno = EINVAL;
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

// TODO: Make returns not be EXIT_SUCCESS or EXIT_FAILURE
int DisableSignalBuildInTasks(int signal) {
	switch(signal) {
		case SIGINT: {
			DATA.SignalData.SIG_INT.enabledBuildInTasks = false;
			break;
		}

		case SIGWINCH: {
			DATA.SignalData.SIG_WINCH.enabledBuildInTasks = false;
			break;
		}
		
		default: {
			TraceLog("[ESCLIB.DisableSignalBuildInTasks]: ERROR: Couldn't disable build in task becouse signal is NOT supported");
			errno = EINVAL;
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

int AddSignalTask(int signal, void (*taskFunction)(void), int index) {
	switch(signal) {
		case SIGINT: {
			int targetIndex = (index < 0) ? DATA.SignalData.SIG_INT.customTasksNumber : index;

			if(targetIndex >= DATA.SignalData.SIG_INT.customTasksNumber) {
				int newSize = targetIndex + 1;

				void (**tmp)(void) = realloc(DATA.SignalData.SIG_INT.customTasks, newSize * sizeof( void (*)(void) ));

				if(!tmp) {
					TraceLog("[ESCLIB.AddSignalTask]: ERROR: Realloc failed for tmp in SIGINT");
					errno = ENOMEM;
					return -1;
				}

				// NOTE: Filling up empty spaces(if 1,2,3 and Add(a,10) than: 1,2,3,NULL,NULL,...,a)
				for(int i = DATA.SignalData.SIG_INT.customTasksNumber; i < newSize; i++) {
                    tmp[i] = NULL;
                }

				DATA.SignalData.SIG_INT.customTasks = tmp;
				DATA.SignalData.SIG_INT.customTasksNumber = newSize;
			}

			DATA.SignalData.SIG_INT.customTasks[targetIndex] = taskFunction;

			return targetIndex;
		}

		case SIGWINCH: {
			int targetIndex = (index < 0) ? DATA.SignalData.SIG_WINCH.customTasksNumber : index;

			if(targetIndex >= DATA.SignalData.SIG_WINCH.customTasksNumber) {
				int newSize = targetIndex + 1;

				void (**tmp)(void) = realloc(DATA.SignalData.SIG_WINCH.customTasks, newSize * sizeof( void (*)(void) ));

				if(!tmp) {
					TraceLog("[ESCLIB.AddSignalTask]: ERROR: Realloc failed for tmp in SIGWINCH");
					errno = ENOMEM;
					return -1;
				}

				// NOTE: Filling up empty spaces(if 1,2,3 and Add(a,10) than: 1,2,3,NULL,NULL,...,a)
				for(int i = DATA.SignalData.SIG_WINCH.customTasksNumber; i < newSize; i++) {
                    tmp[i] = NULL;
                }

				DATA.SignalData.SIG_WINCH.customTasks = tmp;
				DATA.SignalData.SIG_WINCH.customTasksNumber = newSize;
			}

			DATA.SignalData.SIG_WINCH.customTasks[targetIndex] = taskFunction;

			return targetIndex;
		}

		default: {
			TraceLog("[ESCLIB.AddSignalTask]: ERROR: Couldn't add task becouse signal is NOT supported");
			errno = EINVAL;
			return -1;
		}
	}
}

int RemoveSignalTask(int signal, int index) {
	switch(signal) {
		case SIGINT: {
			if(DATA.SignalData.SIG_INT.customTasksNumber == 0) {
				TraceLog("[ESCLIB.RemoveSignalTask]: ERROR: There is no SIGINT tasks to remove");
				return -1;
			}
			if(index < 0) { index = DATA.SignalData.SIG_INT.customTasksNumber - 1; }
			if(index < 0 || index >= DATA.SignalData.SIG_INT.customTasksNumber) {
				TraceLog("[ESCLIB.RemoveSignalTask]: ERROR: There is no SIGINT task with given index to remove");
				errno = EINVAL;
				return -1;
			}

			for(int i = index; i < DATA.SignalData.SIG_INT.customTasksNumber - 1; i++) {
				DATA.SignalData.SIG_INT.customTasks[i] = DATA.SignalData.SIG_INT.customTasks[i + 1];
			}

			DATA.SignalData.SIG_INT.customTasksNumber--;

			if(DATA.SignalData.SIG_INT.customTasksNumber == 0) {
				free(DATA.SignalData.SIG_INT.customTasks);
				DATA.SignalData.SIG_INT.customTasks = NULL;
			}
			else {
				void* tmp = realloc(DATA.SignalData.SIG_INT.customTasks, DATA.SignalData.SIG_INT.customTasksNumber * sizeof( void (*)(void) ));

				if(!tmp) {
					TraceLog("[ESCLIB.RemoveSignalTask]: ERROR: Realloc failed for tmp in SIGINT");
					errno = ENOMEM;
					return -1;
				}

				DATA.SignalData.SIG_INT.customTasks = tmp;
			}

			return 0;
		}

		case SIGWINCH: {
			if(DATA.SignalData.SIG_WINCH.customTasksNumber == 0) {
				TraceLog("[ESCLIB.RemoveSignalTask]: ERROR: There is no SIGWINCH tasks to remove");
				return -1;
			}
			if(index < 0) { index = DATA.SignalData.SIG_WINCH.customTasksNumber - 1; }
			if(index < 0 || index >= DATA.SignalData.SIG_WINCH.customTasksNumber) {
				TraceLog("[ESCLIB.RemoveSignalTask]: ERROR: There is no SIGWINCH task with given index to remove");
				errno = EINVAL;
				return -1;
			}

			for(int i = index; i < DATA.SignalData.SIG_WINCH.customTasksNumber - 1; i++) {
				DATA.SignalData.SIG_WINCH.customTasks[i] = DATA.SignalData.SIG_WINCH.customTasks[i + 1];
			}

			DATA.SignalData.SIG_WINCH.customTasksNumber--;

			if(DATA.SignalData.SIG_WINCH.customTasksNumber == 0) {
				free(DATA.SignalData.SIG_WINCH.customTasks);
				DATA.SignalData.SIG_WINCH.customTasks = NULL;
			}
			else {
				void* tmp = realloc(DATA.SignalData.SIG_WINCH.customTasks, DATA.SignalData.SIG_WINCH.customTasksNumber * sizeof( void (*)(void) ));

				if(!tmp) {
					TraceLog("[ESCLIB.RemoveSignalTask]: ERROR: Realloc failed for tmp in SIGWINCH");
					errno = ENOMEM;
					return -1;
				}

				DATA.SignalData.SIG_WINCH.customTasks = tmp;
			}

			return 0;
		}

		default: {
			TraceLog("[ESCLIB.RemoveSignalTask]: ERROR: Couldn't remove task becouse signal is NOT supported");
			errno = EINVAL;
			return -1;
		}
	}
}

int CompressSignalTasks(int signal, int from, int to) {
	switch(signal) {
		case SIGINT: {
			if(from < 0) { from = 0; }
			if(to < 0 || to > DATA.SignalData.SIG_INT.customTasksNumber) { to = DATA.SignalData.SIG_INT.customTasksNumber; }
			if(from >= to) {
				return 0;
			}

			int writeINDXIO = from;

			for(int i = from; i < to; i++) {
				if(DATA.SignalData.SIG_INT.customTasks[i] != NULL) {
					DATA.SignalData.SIG_INT.customTasks[writeINDXIO] = DATA.SignalData.SIG_INT.customTasks[i];
					if(writeINDXIO != i) {
						DATA.SignalData.SIG_INT.customTasks[i] = NULL;
					}
					writeINDXIO++;
				}
			}

			int newSize = DATA.SignalData.SIG_INT.customTasksNumber - (to - writeINDXIO);

			if(newSize == 0) {
				free(DATA.SignalData.SIG_INT.customTasks);
				DATA.SignalData.SIG_INT.customTasks = NULL;
			}
			else {
				void* tmp = realloc(DATA.SignalData.SIG_INT.customTasks, newSize * sizeof( void(*)(void) ));
				if(!tmp) {
					TraceLog("[ESCLIB.CompressSignalTasks]: ERROR: Realloc failed for tmp in SIGINT");
					errno = ENOMEM;
					return -1;
				}

				DATA.SignalData.SIG_INT.customTasks = tmp;
			}

			DATA.SignalData.SIG_INT.customTasksNumber = newSize;

			return newSize;
		}

		case SIGWINCH: {
			if(from < 0) { from = 0; }
			if(to < 0 || to > DATA.SignalData.SIG_WINCH.customTasksNumber) { to = DATA.SignalData.SIG_WINCH.customTasksNumber; }
			if(from >= to) {
				return 0;
			}

			int writeINDXIO = from;

			for(int i = from; i < to; i++) {
				if(DATA.SignalData.SIG_WINCH.customTasks[i] != NULL) {
					DATA.SignalData.SIG_WINCH.customTasks[writeINDXIO] = DATA.SignalData.SIG_WINCH.customTasks[i];
					if(writeINDXIO != i) {
						DATA.SignalData.SIG_WINCH.customTasks[i] = NULL;
					}
					writeINDXIO++;
				}
			}

			int newSize = DATA.SignalData.SIG_WINCH.customTasksNumber - (to - writeINDXIO);

			if(newSize == 0) {
				free(DATA.SignalData.SIG_WINCH.customTasks);
				DATA.SignalData.SIG_WINCH.customTasks = NULL;
			}
			else {
				void* tmp = realloc(DATA.SignalData.SIG_WINCH.customTasks, newSize * sizeof( void(*)(void) ));
				if(!tmp) {
					TraceLog("[ESCLIB.CompressSignalTasks]: ERROR: Realloc failed for tmp in SIGWINCH");
					errno = ENOMEM;
					return -1;
				}

				DATA.SignalData.SIG_WINCH.customTasks = tmp;
			}

			DATA.SignalData.SIG_WINCH.customTasksNumber = newSize;

			return newSize;
		}

		default: {
			TraceLog("[ESCLIB.CompressSignalTasks]: ERROR: Couldn't compress tasks becouse signal is NOT supported");
			errno = EINVAL;
			return -1;
		}
	}
}
