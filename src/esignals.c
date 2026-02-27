#include "../include/esclib.h"

#include "./private/coredata.h"
#include "./private/common_utils.h"

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
			UniWrite(UNI_WRITE_TARGET_STDOUT, "ERROR: Couldn't enable signal becouse signal is NOT supported", 61);
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
			UniWrite(UNI_WRITE_TARGET_STDOUT, "ERROR: Couldn't disable signal becouse signal is NOT supported", 62);
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
			UniWrite(UNI_WRITE_TARGET_STDOUT, "ERROR: Couldn't enable ESClib task becouse signal is NOT supported", 66);
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
			UniWrite(UNI_WRITE_TARGET_STDOUT, "ERROR: Couldn't disable ESClib task becouse signal is NOT supported", 67); // 67!?
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
			UniWrite(UNI_WRITE_TARGET_STDOUT, "ERROR: Couldn't enable custom task becouse signal is NOT supported", 66);
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
			UniWrite(UNI_WRITE_TARGET_STDOUT, "ERROR: Couldn't disable custom task becouse signal is NOT supported", 67); // 67!?
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
			UniWrite(UNI_WRITE_TARGET_STDOUT, "ERROR: Couldn't enable build in task becouse signal is NOT supported", 68);
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
			UniWrite(UNI_WRITE_TARGET_STDOUT, "ERROR: Couldn't disable build in task becouse signal is NOT supported", 69); // 69!?
			errno = EINVAL;
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}
