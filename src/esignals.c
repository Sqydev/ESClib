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
			UniWriteLen(UNI_WRITE_TARGET_STDOUT, "ERROR: Couldn't enable signal becouse signal is NOT supported\n");
			errno = EINVAL;
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

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
			UniWriteLen(UNI_WRITE_TARGET_STDOUT, "ERROR: Couldn't disable signal becouse signal is NOT supported\n");
			errno = EINVAL;
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

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
			UniWriteLen(UNI_WRITE_TARGET_STDOUT, "ERROR: Couldn't enable ESClib task becouse signal is NOT supported\n");
			errno = EINVAL;
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

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
			UniWriteLen(UNI_WRITE_TARGET_STDOUT, "ERROR: Couldn't disable ESClib task becouse signal is NOT supported\n");
			errno = EINVAL;
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

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
			UniWriteLen(UNI_WRITE_TARGET_STDOUT, "ERROR: Couldn't enable custom task becouse signal is NOT supported\n");
			errno = EINVAL;
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

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
			UniWriteLen(UNI_WRITE_TARGET_STDOUT, "ERROR: Couldn't disable custom task becouse signal is NOT supported\n");
			errno = EINVAL;
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

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
			UniWriteLen(UNI_WRITE_TARGET_STDOUT, "ERROR: Couldn't enable build in task becouse signal is NOT supported\n");
			errno = EINVAL;
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

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
			UniWriteLen(UNI_WRITE_TARGET_STDOUT, "ERROR: Couldn't disable build in task becouse signal is NOT supported\n");
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
					UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: AddSignalTask() realloc failed for tmp in SIGINT\n");
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
					UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: AddSignalTask() realloc failed for tmp in SIGWINCH\n");
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
			UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: Couldn't add task becouse signal is NOT supported\n");
			errno = EINVAL;
			return -1;
		}
	}
}

int RemoveSignalTask(int signal, int index) {
	switch(signal) {
		case SIGINT: {
			if(DATA.SignalData.SIG_INT.customTasksNumber == 0) {
				UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: There is no SignalTasks to RemoveSignalTask()!?\n");
				return -1;
			}
			if(index < 0) { index = DATA.SignalData.SIG_INT.customTasksNumber - 1; }
			if(index < 0 || index >= DATA.SignalData.SIG_INT.customTasksNumber) {
				UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: There is no SignalTask with given index to remove with RemoveSignalTask() :0\n");
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
					UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: RemoveSignalTask() realloc failed for tmp in SIGINT\n");
					errno = ENOMEM;
					return -1;
				}

				DATA.SignalData.SIG_INT.customTasks = tmp;
			}

			return 0;
		}

		case SIGWINCH: {
			if(DATA.SignalData.SIG_WINCH.customTasksNumber == 0) {
				UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: There is no SignalTasks to RemoveSignalTask()!?\n");
				return -1;
			}
			if(index < 0) { index = DATA.SignalData.SIG_WINCH.customTasksNumber - 1; }
			if(index < 0 || index >= DATA.SignalData.SIG_WINCH.customTasksNumber) {
				UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: There is no SignalTask with given index to remove with RemoveSignalTask() :0\n");
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
					UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: RemoveSignalTask() realloc failed for tmp in SIGWINCH\n");
					errno = ENOMEM;
					return -1;
				}

				DATA.SignalData.SIG_WINCH.customTasks = tmp;
			}

			return 0;
		}

		default: {
			UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: Couldn't remove task becouse signal is NOT supported\n");
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
				UniWriteLen(UNI_WRITE_TARGET_STDERR, "WARNING: Nothing to do, from and to are the same. CompressSignalTasks out:)\n");
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
					UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: CompressSignalTasks() realloc failed for tmp in SIGINT\n");
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
				UniWriteLen(UNI_WRITE_TARGET_STDERR, "WARNING: Nothing to do, from and to are the same. CompressSignalTasks out:)\n");
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
					UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: CompressSignalTasks() realloc failed for tmp in SIGWINCH\n");
					errno = ENOMEM;
					return -1;
				}

				DATA.SignalData.SIG_WINCH.customTasks = tmp;
			}

			DATA.SignalData.SIG_WINCH.customTasksNumber = newSize;

			return newSize;
		}

		default: {
			UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: Couldn't compress tasks becouse signal is NOT supported\n");
			errno = EINVAL;
			return -1;
		}
	}
}
